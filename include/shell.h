#include "parser.h"
#include <unistd.h>

typedef struct myshell {

    CommandParser parser;
    int** pipes;
    pid_t curr_proc;
    int bg_procs;

} MyShell;
