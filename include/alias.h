#pragma once

#include "parser.h"

// The struct to hold all the
// alias related information
typedef struct {

    char*** table;  // keeps the pairs alias-command
    int records;    // number of pairs
    
} Aliases;

void aliases_init(Aliases* altable);
void aliases_destroy(Aliases* altable);
int found_alias(char* str, Aliases* altable);
void replace_alias(CommandParser* parser, Aliases* altable, int alias_no);
void sys_createalias(CommandParser* parser, Aliases* altable);
void sys_destroyalias(CommandParser* parser, Aliases* altable);
void sys_printaliases(CommandParser* parser, Aliases* altable);
