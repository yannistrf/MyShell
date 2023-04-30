#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sys_cmd.h"
#include "alias.h"
#include "history.h"
#include "help.h"

// Given a string, returns the sys_cmd, if there is one
// If there is not one returns 0
SysCmd is_sys_cmd(char* cmd) {
    if (!strcmp(cmd, "exit"))
        return EXIT;
    
    if (!strcmp(cmd, "help"))
        return HELP;
    
    if (!strcmp(cmd, "cd"))
        return CD;

    if (!strcmp(cmd, "history"))
        return HISTORY;
    
    if (!strcmp(cmd, "createalias"))
        return CREATEALIAS;
    
    if (!strcmp(cmd, "destroyalias"))
        return DESTROYALIAS;

    if (!strcmp(cmd, "printaliases"))
        return PRINTALIASES;
    
    return 0;
}

// Implementation of exit command
void sys_exit(MyShell* sh) {
    shell_destroy(sh);
    printf("Exiting...\n");
    exit(EXIT_SUCCESS);
}

// Implementation of cd command
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

// Responsible to execute the given system command
// with the appropriate arguments
void exec_sys_cmd(SysCmd cmd, MyShell* sh) {
    switch (cmd) {
        case EXIT:
            sys_exit(sh);
            break;
        case HELP:
            help(&sh->parser);
            break;
        case CD:
            sys_cd(&sh->parser);
            break;
        case HISTORY:
            print_history(&sh->parser, &sh->history);
            break;
        case CREATEALIAS:
            create_alias(&sh->parser, &sh->aliases);
            break;
        case DESTROYALIAS:
            destroy_alias(&sh->parser, &sh->aliases);
            break;
        case PRINTALIASES:
            print_aliases(&sh->parser, &sh->aliases);
            break;
        default:
            break;
    }
}
