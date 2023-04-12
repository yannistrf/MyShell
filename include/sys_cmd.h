#pragma once

#include "parser.h"
#include "util.h"

typedef enum {
    EXIT=1,
    CD,
    CREATEALIAS,
    DESTROYALIAS,
    HISTORY
} SysCmd;

SysCmd is_sys_cmd(char* cmd);
void exec_sys_cmd(SysCmd cmd, CommandParser* parser);
