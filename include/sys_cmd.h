#pragma once

#include "parser.h"
#include "pipes.h"
#include "util.h"
#include "alias.h"

typedef enum {
    EXIT=1,
    CD,
    CREATEALIAS,
    DESTROYALIAS,
    PRINTALIASES,
    HISTORY
} SysCmd;

SysCmd is_sys_cmd(char* cmd);
void exec_sys_cmd(SysCmd cmd, CommandParser* parser, int** pipes, AliasTable* htable);
