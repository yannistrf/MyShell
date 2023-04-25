#pragma once

#include <regex.h>
#include "parser.h"

typedef struct {

    char** table;
    int max_size;
    int size;
    regex_t regex;

} History;

void history_init(History* history);
int replace_history(History* history, CommandParser* parser);
void history_destroy(History* history);
void print_history(CommandParser* parser, History* history);
void history_insert(History* history, char* line);
