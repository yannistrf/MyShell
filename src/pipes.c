#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipes.h"
#include "util.h"

#define READ 0
#define WRITE 1

int** pipes_init(int num_of_pipes) {

    if (num_of_pipes < 0)
        return NULL;

    int** pipes = malloc(sizeof(int*) * num_of_pipes);
    
    for (int pipe_no = 0; pipe_no < num_of_pipes; pipe_no++) {
        pipes[pipe_no] = malloc(sizeof(int*) * 2);
        
        if (pipe(pipes[pipe_no]) == -1) {
            perror("pipe");
            return NULL;
        }
    }

    return pipes;
}

void destroy_pipes(int** pipes, int size) {
    free_list((void**) pipes, size);
}

void handle_pipes(int** pipes, int piped_proc_no, int num_of_pipes) {
    if (piped_proc_no != 0) {
        dup2(pipes[piped_proc_no-1][READ], 0);
        close(pipes[piped_proc_no-1][READ]);
    }

    if (piped_proc_no != num_of_pipes) {
        dup2(pipes[piped_proc_no][WRITE], 1);
        close(pipes[piped_proc_no][WRITE]);
    }
}