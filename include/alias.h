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
void create_alias(CommandParser* parser, Aliases* altable);
void destroy_alias(CommandParser* parser, Aliases* altable);
void print_aliases(CommandParser* parser, Aliases* altable);
int found_alias(char* str, Aliases* altable);
void replace_alias(CommandParser* parser, Aliases* altable, int alias_no);
