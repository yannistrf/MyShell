#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sys_cmd.h"
#include "alias.h"

SysCmd is_sys_cmd(char* cmd) {
    if (!strcmp(cmd, "exit"))
        return EXIT;
    
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

void sys_exit(CommandParser* parser, int** pipes, AliasTable* altable) {
    parser_destroy(parser);
    destroy_pipes(pipes, parser->pipe_commands_size-1);
    alias_table_destroy(altable);
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

void sys_history(CommandParser* parser) {
    if (parser->arg_size > 1) {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }

    char* file_name = ".mysh_history";
    FILE* hist_file = fopen(file_name, "r");
    if (hist_file == NULL) {
        fprintf(stderr, "history: .mysh_history file not found\n");
        return;
    }

    int MAX_LINE_SIZE = 256;
    char line[MAX_LINE_SIZE];
    int count = 1;
    while (fgets(line, MAX_LINE_SIZE, hist_file)) {
        printf("#%d %s", count, line);
        count++;
    }

    fclose(hist_file);
}

void exec_sys_cmd(SysCmd cmd, CommandParser* parser, int** pipes, AliasTable* altable) {
    switch (cmd) {
        case EXIT:
            sys_exit(parser, pipes, altable);
            break;
        case CD:
            sys_cd(parser);
            break;
        case HISTORY:
            sys_history(parser);
            break;
        case CREATEALIAS:
            create_alias(altable, parser);
            break;
        case DESTROYALIAS:
            destroy_alias(parser, altable);
            break;
        case PRINTALIASES:
            print_aliases(parser, altable);
            break;
        default:
            break;
    }
}
