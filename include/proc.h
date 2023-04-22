#pragma once

#include <unistd.h>

#include "shell.h"
#include "parser.h"
#include "alias.h"

int exec_user_cmd(MyShell* sh);
void clean_fg_procs(pid_t* procs, int size);
void clean_bg_procs(int* bg_procs_num);