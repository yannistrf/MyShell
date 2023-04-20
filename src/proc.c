#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "proc.h"
#include "pipes.h"

int exec_user_cmd(CommandParser* parser, int** pipes, AliasTable* altable) {
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        if ((execvp(parser->arguments[0], parser->arguments)) == -1) {
            perror(parser->arguments[0]);
            parser_destroy(parser);
            destroy_pipes(pipes, parser->pipe_commands_size-1);
            alias_table_destroy(altable);
            exit(EXIT_FAILURE);
        }
    }

    return pid;
}

void clean_fg_procs(pid_t* procs, int size) {
    for (int i = 0; i < size; i++) {
        if (procs[i] == 0)
            continue;
        waitpid(procs[i], NULL, 0);
    }
}

void clean_bg_procs(int* bg_procs) {
    int procs_num = *bg_procs;
    for (int i = 0; i < procs_num; i++) {
        if (waitpid(0, NULL, WNOHANG))
            (*bg_procs)--;
    }
}