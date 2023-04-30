#pragma once

#include "shell.h"

/*
    Here is the mapping of every
    system command to an int
*/
typedef enum {

    EXIT=1,     // Start from 1, 0 means not a sys_cmd
    HELP,
    CD,
    CREATEALIAS,
    DESTROYALIAS,
    PRINTALIASES,
    HISTORY
    
} SysCmd;

SysCmd is_sys_cmd(char* cmd);
void exec_sys_cmd(SysCmd cmd, MyShell* sh);
