#include <unistd.h>
#include "parser.h"

int exec_user_cmd(CommandParser* parser, int** pipes);
void clean_fg_procs(pid_t* procs, int size);
void clean_bg_procs(int* bg_procs);