#pragma once

#include <unistd.h>

#include "shell.h"


int exec_user_cmd(MyShell* sh, int run_bg);
void clean_fg_procs(pid_t* procs, int size);
void clean_bg_procs(int* bg_procs_num);