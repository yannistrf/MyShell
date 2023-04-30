#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alias.h"

void aliases_init(Aliases* aliases) {

    aliases->table = NULL;
    aliases->records = 0;
}

void aliases_destroy(Aliases* aliases) {

    for (int i = 0; i < aliases->records; i++) {
        // free alias string
        free(aliases->table[i][0]);
        // free command string
        free(aliases->table[i][1]);
        // free pointer to pair
        free(aliases->table[i]);
    }

    // free table of pairs
    free(aliases->table);
}

int found_alias(char* str, Aliases* aliases) {

    for (int i = 0; i < aliases->records; i++) {
        if (!strcmp(aliases->table[i][0], str))
            return i;
    }

    return -1;
}

// Replaces alias with the appropriate command and updates the arguments
// The alias-command will probably have some arguments of its own so we
// need to mess with the arguments inside the parser
void replace_alias(CommandParser* parser, Aliases* aliases, int alias_no) {

    // We parse the alias-command to get the args
    char* command = malloc(strlen(aliases->table[alias_no][1])+1);
    char* temp = command;
    strcpy(command, aliases->table[alias_no][1]);
    // strtok messes with the given string, give a copy
    char* token = strtok_r(command, " ", &command);

    // Create new arg list
    char** new_args = NULL;
    int new_arg_size = 0;

    // Store inside the new arguments list the alias-command args
    while (token != NULL) {
        new_arg_size++;
        new_args = realloc(new_args, sizeof(char*) * new_arg_size);
        new_args[new_arg_size-1] = malloc(strlen(token)+1);
        strcpy(new_args[new_arg_size-1], token);

        token = strtok_r(NULL, " ", &command);
    }
    free(temp);

    // Add to the new arguments list the already existant args
    new_args = realloc(new_args, sizeof(char*) * (new_arg_size + parser->arg_size));
    free(parser->arguments[0]);
    for (int i = 0; i < parser->arg_size; i++) {
        new_args[i+new_arg_size] = parser->arguments[i+1];
    }

    // Replace the old argument list
    free(parser->arguments);
    parser->arguments = new_args;
    parser->arg_size = new_arg_size - 1 + parser->arg_size;
}

void create_alias(CommandParser* parser, Aliases* aliases) {

    if (parser->arg_size != 3) {
        fprintf(stderr, "createalias: Usage createalias <alias> <command>\n");
        return;
    }

    // Handle the case where there is already an alias with that name
    // Just replace the command part of the pair
    int alias_no;
    if ((alias_no = found_alias(parser->arguments[1], aliases)) != -1) {
        free(aliases->table[alias_no][1]);
        aliases->table[alias_no][1] = malloc(strlen(parser->arguments[2])+1);
        strcpy(aliases->table[alias_no][1], parser->arguments[2]);
        return;
    }

    // Get args and allocate the space for the strings
    char* alias = malloc(strlen(parser->arguments[1])+1);
    strcpy(alias, parser->arguments[1]);
    char* command = malloc(strlen(parser->arguments[2])+1);
    strcpy(command, parser->arguments[2]);

    // Update our struct
    aliases->records++;
    aliases->table = realloc(aliases->table, aliases->records * sizeof(char**));
    aliases->table[aliases->records-1] = malloc(2 * sizeof(char*));
    aliases->table[aliases->records-1][0] = alias;
    aliases->table[aliases->records-1][1] = command;
}

void destroy_alias(CommandParser* parser, Aliases* aliases) {
    
    if (parser->arg_size < 2) {
        fprintf(stderr, "deletealias: too few arguments\n");
        return;
    }

    int alias_no;
    for (int i = 1; i < parser->arg_size; i++) {
        if ((alias_no = found_alias(parser->arguments[i], aliases)) == -1) {
            fprintf(stderr, "destroyalias: %s: there is no such alias\n", parser->arguments[i]);
            continue;
        }

        aliases->records--;
        // Free the space of the deleted alias
        free(aliases->table[alias_no][0]);
        free(aliases->table[alias_no][1]);
        free(aliases->table[alias_no]);

        char*** new_table = malloc(aliases->records * sizeof(char**));

        // Copies all of the pair pointer, besides the one we deleted
        for (int old = 0, new = 0; new < aliases->records; old++,new++) {
            if (new == alias_no)
                old++;

            new_table[new] = aliases->table[old];
        }

        // Replace old table with the updated one
        free(aliases->table);
        aliases->table = new_table;
    }
}

void print_aliases(CommandParser* parser, Aliases* aliases) {

    if (parser->arg_size > 1) {
        fprintf(stderr, "printaliases: too many arguments\n");
        return;
    }

    for (int i = 0; i < aliases->records; i++) {
        printf("%s -> %s\n", aliases->table[i][0], aliases->table[i][1]);
    }
}