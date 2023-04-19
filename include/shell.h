#include <unistd.h>

#include "util.h"
#include "parser.h"

typedef struct myshell {

    char* username;
    char path[PATH_MAX];
    CommandParser parser;
    int** pipes;
    pid_t curr_proc;
    int bg_procs;

} MyShell;
