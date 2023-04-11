#include <stdlib.h>
#include "util.h"

void free_list(char** list, int size) {
    for (int i = 0; i < size; i++)
        free(list[i]);

    free(list);
}

void parser_init(CommandParser* parser) {
    parser->line = NULL;
    parser->semicolon_parsed_list = NULL;
    parser->pipe_parsed_list = NULL;
}


void parser_destroy(CommandParser* parser) {
    free(parser->line);
    free_list(parser->semicolon_parsed_list, parser->semi_size);
    free_list(parser->pipe_parsed_list, parser->pipe_size);
}