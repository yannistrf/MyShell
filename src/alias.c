#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alias.h"

void alias_table_init(AliasTable* altable) {
    altable->table = NULL;
    altable->records = 0;
}

void alias_table_destroy(AliasTable* altable) {
    for (int i = 0; i < altable->records; i++) {
        free(altable->table[i][0]);
        free(altable->table[i][1]);
        free(altable->table[i]);
    }

    free(altable->table);
}

int found_alias(char* str, AliasTable* altable) {
    for (int i = 0; i < altable->records; i++) {
        if (!strcmp(altable->table[i][0], str))
            return i;
    }

    return -1;
}

void replace_alias(CommandParser* parser, AliasTable* altable, int alias_no) {
    char* command = malloc(strlen(altable->table[alias_no][1])+1);
    char* temp = command;
    strcpy(command, altable->table[alias_no][1]);
    char* token = strtok_r(command, " ", &command);

    char** new_args = NULL;
    int new_arg_size = 0;

    while (token != NULL) {
        new_arg_size++;
        new_args = realloc(new_args, sizeof(char*) * new_arg_size);
        new_args[new_arg_size-1] = malloc(strlen(token)+1);
        strcpy(new_args[new_arg_size-1], token);

        token = strtok_r(NULL, " ", &command);
    }
    free(temp);

    new_args = realloc(new_args, sizeof(char*) * (new_arg_size + parser->arg_size));
    free(parser->arguments[0]);
    for (int i = 0; i < parser->arg_size; i++) {
        new_args[i+new_arg_size] = parser->arguments[i+1];
    }

    free(parser->arguments);
    parser->arguments = new_args;
    parser->arg_size = new_arg_size - 1 + parser->arg_size;
}

void create_alias(AliasTable* altable, CommandParser* parser) {
    if (parser->arg_size != 3) {
        fprintf(stderr, "createalias: Usage createalias <alias> <command>\n");
        return;
    }

    char* alias = malloc(strlen(parser->arguments[1])+1);
    strcpy(alias, parser->arguments[1]);
    char* command = malloc(strlen(parser->arguments[2])+1);
    strcpy(command, parser->arguments[2]);

    altable->records++;
    altable->table = realloc(altable->table, altable->records * sizeof(char**));
    altable->table[altable->records-1] = malloc(2 * sizeof(char*));
    altable->table[altable->records-1][0] = alias;
    altable->table[altable->records-1][1] = command;
}

void destroy_alias(CommandParser* parser, AliasTable* altable) {
    if (parser->arg_size < 2) {
        fprintf(stderr, "deletealias: too few arguments\n");
        return;
    }

    int alias_no;
    for (int i = 1; i < parser->arg_size; i++) {
        if ((alias_no = found_alias(parser->arguments[i], altable)) == -1) {
            fprintf(stderr, "destroyalias: %s: there is no such alias\n", parser->arguments[i]);
            continue;
        }

        altable->records--;
        free(altable->table[alias_no][0]);
        free(altable->table[alias_no][1]);
        free(altable->table[alias_no]);
        char*** new_table = malloc(altable->records * sizeof(char**));

        for (int old = 0, new = 0; new < altable->records; old++,new++) {
            if (new == alias_no) {
                old++;
                
            }

            new_table[new] = altable->table[old];
        }

        free(altable->table);
        altable->table = new_table;
    }
}

void print_aliases(CommandParser* parser, AliasTable* altable) {

    if (parser->arg_size > 1) {
        fprintf(stderr, "printaliases: too many arguments\n");
        return;
    }

    for (int i = 0; i < altable->records; i++) {
        printf("%s -> %s\n", altable->table[i][0], altable->table[i][1]);
    }
}