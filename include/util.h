#pragma once

typedef struct command_parser {
    char* line;
    int line_size;
    char** semicolon_parsed_list;
    int semi_size;
    char** pipe_parsed_list;
    int pipe_size;
} CommandParser;

void free_list(char** list, int size);
void parser_init(CommandParser* parser);
void parser_destroy(CommandParser* parser);
