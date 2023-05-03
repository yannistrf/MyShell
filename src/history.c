#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history.h"
#include "util.h"

#define MAXSIZE 20


void history_init(History* history) {

    history->table = malloc(sizeof(char*) * MAXSIZE);
    history->max_size = MAXSIZE;
    history->size = 0;
    // Find substrings of the form "#number" where the digits
    // of the number are at least one
    regcomp(&history->regex, "#[0-9]+", REG_EXTENDED);
}

// Replaces the proper past command to the input user line
// After that, the new command executes normally like if the
// user had provided  that input
int replace_history(History* history, CommandParser* parser) {

    regmatch_t pmatch[1];
    regoff_t off, len;
    char* hist_str;
    int hist_no;
    char* hist_cmd;

    // May want to access multiple past commands
    while (!regexec(&history->regex, parser->line, 1, pmatch, 0)) {
        char* old_line = parser->line;
        off = pmatch[0].rm_so;
        len = pmatch[0].rm_eo - pmatch[0].rm_so;
        
        // Get the number of the history command
        hist_str = malloc(sizeof(len)+1);
        strncpy(hist_str, &parser->line[off], len);
        hist_str[len] = '\0';
        hist_no = atoi(hist_str+1);

        if (hist_no > history->size) {
            fprintf(stderr, "#%d: history number too big\n", hist_no);
            free(hist_str);
            return -1;
        }

        // Get the command from the history table
        hist_cmd = history->table[hist_no];
        // Calculate the new size of the line, having replaced the number with the command
        parser->line_size = strlen(old_line) - strlen(hist_str) + strlen(hist_cmd);
        parser->line_buf_size = parser->line_size + 1;
        parser->line = malloc(parser->line_buf_size);
        // Copy the contents of the old line until the number
        strncpy(parser->line, old_line, off);
        // Copy the command from the history table
        strncpy(parser->line+off, hist_cmd, strlen(hist_cmd));
        // Copy the contents of the old line after the number
        strncpy(parser->line+off+strlen(hist_cmd), old_line+off+len, strlen(old_line+off+len));
        // Terminate with null
        parser->line[parser->line_size] = '\0';

        free(hist_str);
        free(old_line);

    }

    return 1;
}

// Adds the line into the history struct
void history_insert(History* history, char* line) {

    char** new_table = malloc(sizeof(char*) * history->max_size);
    // Copy old elements, leave first position empty
    // if we reached max size, free the oldest entry
    for (int i = 0; i < history->size; i++)
        if (i+1 < history->max_size) {
            new_table[i+1] = history->table[i];
        }
        else {
            free(history->table[history->max_size-1]);
            history->size--;
        }

    // Insert new command
    new_table[0] = malloc(strlen(line)+1);
    strcpy(new_table[0], line);
    free(history->table);
    history->table = new_table;
    history->size++;
}

void history_destroy(History* history) {

    regfree(&history->regex);
    free_list((void**) history->table, history->size);
}

// Prints the history table
void sys_history(CommandParser* parser, History* history) {

    if (parser->arg_size > 1) {
        fprintf(stderr, "history: too many arguments\n");
        return;
    }

    printf("\n*** HISTORY ***\n");
    printf("---------------\n");

    for (int i = 0; i < history->size; i++) {
        printf("#%d %s\n", i, history->table[i]);
    }
}