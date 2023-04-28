#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pipes.h"
#include "util.h"

#define READ 0
#define WRITE 1

void pipes_init(Pipes* pipes) {

    if (pipes->size < 0)
        return;

    pipes->table = malloc(sizeof(int*) * pipes->size);
    
    for (int pipe_no = 0; pipe_no < pipes->size; pipe_no++) {

        pipes->table[pipe_no] = malloc(sizeof(int) * 2);
        if (pipe(pipes->table[pipe_no]) == -1) {
            perror("pipe");
            return;
        }
    }
}

void destroy_pipes(Pipes* pipes) {

    free_list((void**) pipes->table, pipes->size);
}

void handle_pipes(Pipes* pipes, int proc_no) {
    // Read from the previous process
    if (proc_no != 0) {
        dup2(pipes->table[proc_no-1][READ], 0);
        close(pipes->table[proc_no-1][READ]);
    }

    // Write to the next process
    if (proc_no != pipes->size) {
        dup2(pipes->table[proc_no][WRITE], 1);
        close(pipes->table[proc_no][WRITE]);
    }
}