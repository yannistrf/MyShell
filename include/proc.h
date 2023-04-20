#pragma once

#include <unistd.h>
#include "parser.h"
#include "alias.h"

int exec_user_cmd(CommandParser* parser, int** pipes, AliasTable* altable);
void clean_fg_procs(pid_t* procs, int size);
void clean_bg_procs(int* bg_procs);