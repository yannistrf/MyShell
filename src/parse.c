#include <string.h>
#include <stdlib.h>
#include "parse.h"


int empty_line(char* line) {
    if (strlen(line) == 0)
        return 1;

    int i = 0;
    while (line[i] != '\0') {
        if (line[i] != ' ' && line[i] != '\t')
            return 0;
        i++;
    }

    return 1;
}

char* remove_spaces(char* str) {
    while(*str == ' ')
        str++;

    int end = strlen(str)-1;
    while (str[end] == ' ')
        end--;
    
    str[end+1] = '\0';
    return str;
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

int is_special_char(char* token) {
    token = remove_spaces(token);
    return !strcmp(token, ">") || !strcmp(token, ">>") || !strcmp(token, "&");
}

char** get_arguments(char* program, int* size) {
    char* token = strtok_r(program, " ", &program);
    *size = 0;

    char** token_list = NULL;

    while (token != NULL) {
        token = remove_spaces(token);
        if (is_special_char(token))
            break;
        (*size)++;
        token_list = realloc(token_list, sizeof(char*) * (*size));
        token_list[*size-1] = malloc(strlen(token)+1);
        strcpy(token_list[*size-1], token);
        token = strtok_r(NULL, " ", &program);
    }

    return token_list;   
}