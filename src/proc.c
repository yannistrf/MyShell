#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "proc.h"
#include "sig.h"

int exec_user_cmd(MyShell* sh, int run_bg) {

    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // if a process runs bg then we want to ignore the signals
        if (!run_bg)
            sig_handle_child();
        if ((execvp(sh->parser.arguments[0], sh->parser.arguments)) == -1) {
            perror(sh->parser.arguments[0]);
            shell_destroy(sh);
            exit(EXIT_FAILURE);
        }
    }

    return pid;
}

// Cleans up all the processes that were running in
// the foreground simultaneously
void clean_fg_procs(pid_t* procs, int size) {

    for (int i = 0; i < size; i++) {
        // Don't wait for sys commands
        if (procs[i] == 0)
            continue;
        waitpid(procs[i], NULL, WUNTRACED);
    }
}

// Cleans up any processes that are finished and
// were running in the background
void clean_bg_procs(int* bg_procs_num) {

    int procs_num = *bg_procs_num;
    for (int i = 0; i < procs_num; i++) {
        // If no process has finished, don't block
        if (waitpid(0, NULL, WNOHANG))
            (*bg_procs_num)--;
    }
}