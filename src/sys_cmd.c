#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sys_cmd.h"

SysCmd is_sys_cmd(char* cmd) {
    if (!strcmp(cmd, "exit"))
        return EXIT;
    
    if (!strcmp(cmd, "cd"))
        return CD;

    return 0;
}

void sys_exit(CommandParser* parser) {
    parser_destroy(parser);
    printf("Exiting...\n");
    exit(EXIT_SUCCESS);
}

void sys_cd(CommandParser* parser) {
    if (parser->arg_size > 2) {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }

    char* path;
    if (parser->arg_size == 1)
        path = getenv("HOME");
    else
        path = parser->arguments[1];

    if (chdir(path) == -1)
        perror("cd");

}

void sys_createalias() {}

void sys_destroyalias() {}

void sys_history() {}

void exec_sys_cmd(SysCmd cmd, CommandParser* parser) {
    switch (cmd) {
        case EXIT:
            sys_exit(parser);
            break;
        case CD:
            sys_cd(parser);
            break;
        default:
            break;
    }
}
