#include <stdlib.h>
#include <string.h>
#include <glob.h>

#include "wildchar.h"
#include "util.h"

// Checks if the string has a wild character in it
int has_wildchar(const char* str) {

    int len = strlen(str);

    for (int i = 0; i < len; i++) {
        char ch = str[i];
        if (ch == '*' || ch == '?' || ch == '[')
            return 1;
    }

    return 0;
}

// Returns a list with the indexes of the arguments that were identified as wild patterns.
// Returns NULL otherwise
int* found_wildchar(CommandParser* parser, int* wild_args_found) {
    
    int* wild_args_no = NULL;
    *wild_args_found = 0;

    for (int i = 1; i < parser->arg_size; i++) {
        if (has_wildchar(parser->arguments[i])) {
           (*wild_args_found)++;
            wild_args_no = realloc(wild_args_no, sizeof(int) * (*wild_args_found));
            wild_args_no[*wild_args_found-1] = i;
        }
    }

    return wild_args_no;
}

// Changes the arguments inside the parser according to the
// successful matches returned from glob
void replace_wildchars(CommandParser* parser) {

    int* wild_args;     // store the indexes of the wildchar arguments here
    int wild_args_found;
    int wild_nomatch_count = 0;

    if ((wild_args = found_wildchar(parser, &wild_args_found)) == NULL)
        return;

    glob_t globbuf;
    for (int i = 0; i < wild_args_found; i++) {
        int gl_flags = GLOB_APPEND;
        if (i == 0) gl_flags = 0;   // first call of glob shouldn't have GLOB_APPEND
        
        if (glob(parser->arguments[wild_args[i]], gl_flags, NULL, &globbuf) == GLOB_NOMATCH) {
            wild_args[i] *= -1;     // negative means the wild arg didn't have any matches
            wild_nomatch_count++;
        }
    }

    // new_size = (old arg size) - (num of wild args) + (num of successful matches) + (num of unsuccessful matches size)
    int new_size = parser->arg_size - wild_args_found + globbuf.gl_pathc + wild_nomatch_count;
    char** new_args = malloc(sizeof(char*) * (new_size+1));


    // Copy all arguments, except the ones that returned from glob
    for (int i = 0, j = 0, k = 0; i < new_size - globbuf.gl_pathc; i++, k++) {
        // found wild arg
        // check j to not exceed the bounds of wild_args
        if (j < wild_args_found && (k == wild_args[j] || k == (-1) * wild_args[j])) {
            j++;
            // do not copy if it returned successful matches
            // if it returned no successful matches, handle it like a normal arg
            if (wild_args[j-1] > 0) {
                i--;
                continue;
            }
        }

        new_args[i] = malloc(strlen(parser->arguments[k]) + 1);
        strcpy(new_args[i], parser->arguments[k]);
    }

    // Copy the results from glob
    for (int i = new_size - globbuf.gl_pathc, j = 0; i < new_size; i++, j++) {
        new_args[i] = malloc(strlen(globbuf.gl_pathv[j]) + 1);
        strcpy(new_args[i], globbuf.gl_pathv[j]);
    }

    // Update arguments on parser
    free_list((void**) parser->arguments, parser->arg_size);
    parser->arg_size = new_size;
    parser->arguments = new_args;
    parser->arguments[parser->arg_size] = NULL;

    free(wild_args);
    globfree(&globbuf);
}