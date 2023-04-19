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

void semicolon_separation(CommandParser* parser) {
    char* line = parser->line;
    char* token = strtok_r(line, ";", &line);

    if (parser->semicolon_parsed_list != NULL)
        free_list((void**) parser->semicolon_parsed_list, parser->semi_commands_size);
    
    parser->semicolon_parsed_list = NULL;
    parser->semi_commands_size = 0;

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
}


void pipe_separation(CommandParser* parser, int command_no) {
    char* command = parser->semicolon_parsed_list[command_no];
    char* token = strtok_r(command, "|", &command);

    if (parser->pipe_parsed_list != NULL)
        free_list((void**) parser->pipe_parsed_list, parser->pipe_commands_size);

    parser->pipe_parsed_list = NULL;
    parser->pipe_commands_size = 0;

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
}


void get_arguments(CommandParser* parser, int pipe_no) {

    char* command = malloc(strlen(parser->pipe_parsed_list[pipe_no])+1);
    char* temp = command;
    strcpy(command, parser->pipe_parsed_list[pipe_no]);
    char* token = strtok_r(command, " ", &command);

    if (parser->arguments != NULL)
        free_list((void**) parser->arguments, parser->arg_size);

    parser->arguments = NULL;
    parser->arg_size = 0;

    while (token != NULL) {
        token = remove_spaces(token);

        if (is_special_char(token))
            break;
        
        parser->arg_size++;
        parser->arguments = realloc(
                parser->arguments,
                sizeof(char*) * parser->arg_size
            );
        parser->arguments[parser->arg_size-1] = malloc(strlen(token)+1);
        strcpy(parser->arguments[parser->arg_size-1], token);
        token = strtok_r(NULL, " ", &command);
    }

    parser->arguments = realloc(parser->arguments, sizeof(char*) * (parser->arg_size+1));
    parser->arguments[parser->arg_size] = NULL;
    free(temp);
}

int handle_redirections(CommandParser* parser, int pipe_no) {
    
    char* command = parser->pipe_parsed_list[pipe_no];
    char* token = strtok_r(command, " ", &command);

    while(token != NULL) {
        if (!strcmp(token, ">")) {
            token = strtok_r(NULL, " ", &command);
            int fd;
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

    return 0;
}

int run_bg(char* command) {
    int str_size = strlen(command);
    char ch;
    for (int i = str_size-1; i >= 0; i--) {
        ch = command[i];
        if (ch == ' ')
            continue;
        
        if (ch == '&')
            return 1;

        return 0;
    }

    return 0;
}