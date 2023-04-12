#include <string.h>
#include <stdlib.h>
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
    free_list(parser->semicolon_parsed_list, parser->semi_size);
    free_list(parser->pipe_parsed_list, parser->pipe_size);
    free_list(parser->arguments, parser->arg_size);
}

void semicolon_separation(CommandParser* parser) {
    char* line = parser->line;
    char* token = strtok_r(line, ";", &line);

    if (parser->semicolon_parsed_list != NULL)
        free_list(parser->semicolon_parsed_list, parser->semi_size);
    
    parser->semicolon_parsed_list = NULL;
    parser->semi_size = 0;

    while (token != NULL) {
        token = remove_spaces(token);
        parser->semi_size++;
        parser->semicolon_parsed_list = realloc(
                parser->semicolon_parsed_list,
                sizeof(char*) * parser->semi_size
            );
        parser->semicolon_parsed_list[parser->semi_size-1] = malloc(strlen(token)+1);
        strcpy(parser->semicolon_parsed_list[parser->semi_size-1], token);
        token = strtok_r(NULL, ";", &line);
    }
}


void pipe_separation(CommandParser* parser, int command_no) {
    char* command = parser->semicolon_parsed_list[command_no];
    char* token = strtok_r(command, "|", &command);

    if (parser->pipe_parsed_list != NULL)
        free_list(parser->pipe_parsed_list, parser->pipe_size);

    parser->pipe_parsed_list = NULL;
    parser->pipe_size = 0;

    while (token != NULL) {
        token = remove_spaces(token);
        parser->pipe_size++;
        parser->pipe_parsed_list = realloc(
                parser->pipe_parsed_list,
                sizeof(char*) * parser->pipe_size
            );
        parser->pipe_parsed_list[parser->pipe_size-1] = malloc(strlen(token)+1);
        strcpy(parser->pipe_parsed_list[parser->pipe_size-1], token);
        token = strtok_r(NULL, "|", &command);
    }
}


void get_arguments(CommandParser* parser, int pipe_no) {
    char* command = parser->pipe_parsed_list[pipe_no];
    char* token = strtok_r(command, " ", &command);

    if (parser->arguments != NULL)
        free_list(parser->arguments, parser->arg_size);

    parser->arguments = NULL;
    parser->arg_size = 0;

    while (token != NULL) {
        token = remove_spaces(token);
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
    
}