#pragma once

#include <regex.h>
#include "parser.h"

/*
    Contains all the info needed
    providing the history functionality
*/

typedef struct {

    char** table;   // list of past commands
    int max_size;   // can store up until a certain number of commands
    int size;       // current number of commands stored
    regex_t regex;  // the regex for identifying when the user wants to access a past command

} History;

void history_init(History* history);
int replace_history(History* history, CommandParser* parser);
void history_insert(History* history, char* line);
void history_destroy(History* history);
void sys_history(CommandParser* parser, History* history);
