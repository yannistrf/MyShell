#pragma once

#include "parser.h"

typedef struct alias_table {

    char*** table;
    int records; 

} AliasTable;


void alias_table_init(AliasTable* altable);
void alias_table_destroy(AliasTable* altable);
void create_alias(AliasTable* altable, CommandParser* parser);
void destroy_alias(CommandParser* parser, AliasTable* altable);
void print_aliases(CommandParser* parser, AliasTable* altable);
int found_alias(char* str, AliasTable* altable);
void replace_alias(CommandParser* parser, AliasTable* altable, int alias_no);
