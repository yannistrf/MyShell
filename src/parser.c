#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "parser.h"
#include "util.h"

void parser_init(CommandParser* parser) {

    parser->line = NULL;
    parser->line_buf_size = 0;
    parser->semicolon_parsed_list = NULL;
    parser->pipe_parsed_list = NULL;
    parser->arguments = NULL;
}

void parser_destroy(CommandParser* parser) {

    free(parser->line);
    free_list((void**) parser->semicolon_parsed_list, parser->semi_commands_size);
    free_list((void**) parser->pipe_parsed_list, parser->pipe_commands_size);
    free_list((void**) parser->arguments, parser->arg_size);
}

// This function seperates the given string based on the ';'
// Stores the tokens inside the parser
void semicolon_separation(CommandParser* parser) {

    char* line = malloc(strlen(parser->line)+1);
    char* temp = line;
    strcpy(line, parser->line);
    // strtok messes with the given string, give a copy
    char* token = strtok_r(line, ";", &line);

    // Free the list from the previous parse
    if (parser->semicolon_parsed_list != NULL)
        free_list((void**) parser->semicolon_parsed_list, parser->semi_commands_size);
    
    parser->semicolon_parsed_list = NULL;
    parser->semi_commands_size = 0;

    // Store tokens inside the parser
    while (token != NULL) {
        token = remove_spaces(token);
        parser->semi_commands_size++;
        parser->semicolon_parsed_list = realloc(
                parser->semicolon_parsed_list,
                sizeof(char*) * parser->semi_commands_size
            );
        parser->semicolon_parsed_list[parser->semi_commands_size-1] = malloc(strlen(token)+1);
        strcpy(parser->semicolon_parsed_list[parser->semi_commands_size-1], token);
        token = strtok_r(NULL, ";", &line);
    }

    free(temp);
}

// This function seperates the given string based on the ';'
// Stores the tokens inside the parser
void pipe_separation(CommandParser* parser, int command_no) {

    char* command = malloc(strlen(parser->semicolon_parsed_list[command_no])+1);
    char* temp = command;
    strcpy(command, parser->semicolon_parsed_list[command_no]);
    // strtok messes with the given string, give a copy
    char* token = strtok_r(command, "|", &command);

    // Free the list from the previous parse
    if (parser->pipe_parsed_list != NULL)
        free_list((void**) parser->pipe_parsed_list, parser->pipe_commands_size);

    parser->pipe_parsed_list = NULL;
    parser->pipe_commands_size = 0;

    // Store tokens inside the parser
    while (token != NULL) {
        token = remove_spaces(token);
        parser->pipe_commands_size++;
        parser->pipe_parsed_list = realloc(
                parser->pipe_parsed_list,
                sizeof(char*) * parser->pipe_commands_size
            );
        parser->pipe_parsed_list[parser->pipe_commands_size-1] = malloc(strlen(token)+1);
        strcpy(parser->pipe_parsed_list[parser->pipe_commands_size-1], token);
        token = strtok_r(NULL, "|", &command);
    }

    free(temp);
}

// This function extracts the arguments of a command
// and stores them inside the parser
int get_arguments(CommandParser* parser, int pipe_no) {

    char* command = malloc(strlen(parser->pipe_parsed_list[pipe_no])+1);
    char* temp = command;
    strcpy(command, parser->pipe_parsed_list[pipe_no]);
    // strtok messes with the given string, give a copy
    char* token = strtok_r(command, " ", &command);

    // Free the list from the previous parse
    if (parser->arguments != NULL)
        free_list((void**) parser->arguments, parser->arg_size);

    parser->arguments = NULL;
    parser->arg_size = 0;

    while (token != NULL) {
        // If a special char is found, then we got all of the args
        if (is_special_char(token))
            break;
        
        // Need to take care of a multiword argument
        // surrounded by quotes, for example >> createalias hi "echo hi"
        int found_quotes = 0;
        if (token[0] == '"') {
            found_quotes = 1;
            // Store null like this for strlen and realloc to work properly
            char* multi_word_arg = malloc(1);
            strcpy(multi_word_arg, "\0");
            // Move one char to get rid of the first quote
            token++;
            int end_quotes = 0;
            
            while (!end_quotes) {
                // Found closing quote
                if (token[strlen(token)-1] == '"') {
                    // Get rid of closing quote
                    token[strlen(token)-1] = '\0';
                    end_quotes = 1;
                }

                // Update our argument with the new word
                multi_word_arg = realloc(multi_word_arg, strlen(multi_word_arg) + strlen(token) + 2 - end_quotes);
                strcat(multi_word_arg, token);
                if (!end_quotes) {
                    strcat(multi_word_arg, " ");
                    token = strtok_r(NULL, " ", &command);
                }
            }

            token = multi_word_arg;
        }

        // Store tokens inside the parser
        parser->arg_size++;
        parser->arguments = realloc(
                parser->arguments,
                sizeof(char*) * parser->arg_size
            );
        parser->arguments[parser->arg_size-1] = malloc(strlen(token)+1);
        strcpy(parser->arguments[parser->arg_size-1], token);

        if (found_quotes)
            free(token);

        token = strtok_r(NULL, " ", &command);
        
    }

    parser->arguments = realloc(parser->arguments, sizeof(char*) * (parser->arg_size+1));
    parser->arguments[parser->arg_size] = NULL;
    free(temp);

    return 0;
}

// This functions checks for any file redirections
int handle_redirections(CommandParser* parser, int pipe_no) {
    
    char* command = malloc(strlen(parser->pipe_parsed_list[pipe_no])+1);
    char* temp = command;
    strcpy(command, parser->pipe_parsed_list[pipe_no]);
    // strtok messes with the given string, give a copy
    char* token = strtok_r(command, " ", &command);

    while(token != NULL) {
        if (!strcmp(token, ">")) {
            token = strtok_r(NULL, " ", &command);
            int fd;
            // Get current file permissions
            mode_t mask = umask(0);
            umask(mask);
            if ((fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, ~mask)) == -1) {
                perror("open");
                return -1;
            }
            
            dup2(fd, 1);
            continue;
        }

        else if (!strcmp(token, "<")) {
            token = strtok_r(NULL, " ", &command);
            int fd;
            // Get current file permissions
            mode_t mask = umask(0);
            umask(mask);
            if ((fd = open(token, O_RDONLY)) == -1) {
                perror("open");
                return -1;
            }
            
            dup2(fd, 0);
            continue;
        }

        else if (!strcmp(token, ">>")) {
            token = strtok_r(NULL, " ", &command);
            int fd;
            // Get current file permissions
            mode_t mask = umask(0);
            umask(mask);
            if ((fd = open(token, O_WRONLY | O_CREAT | O_APPEND, ~mask)) == -1) {
                perror("open");
                return -1;
            }
            
            dup2(fd, 1);
            continue;
        }

        token = strtok_r(NULL, " ", &command);
    }

    free(temp);
    return 0;
}

// This functions returns 1 if the & is found at the end
// of the string (background process). Returns 0 otherwise
int run_bg(char* command) {
    int str_size = strlen(command);
    char ch;
    // Start from the end of the string
    for (int i = str_size-1; i >= 0; i--) {
        ch = command[i];
        if (ch == ' ')
            continue;
        
        if (ch == '&' && command[i-1] == ' ')
            return 1;

        return 0;
    }

    return 0;
}