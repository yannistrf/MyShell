#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#include "shell.h"
#include "parser.h"
#include "sys_cmd.h"
#include "util.h"
#include "pipes.h"
#include "proc.h"
#include "alias.h"

int main() {

    MyShell shell;
    shell_init(&shell);
    size_t buf_size = 0;

    while (1) {

        prompt(shell.path, shell.username);
        
        // Allocates space for line, don't forget to free
        shell.parser.line_size = getline(&shell.parser.line, &buf_size, stdin) - 1;

        // Terminate string one char earlier, because of '\n'
        shell.parser.line[shell.parser.line_size] = '\0';

        if (empty_line(shell.parser.line))
            continue;
        
        semicolon_separation(&shell.parser);
        // Loop through the ";" seperated tokens
        for (int semi_command_no = 0; semi_command_no < shell.parser.semi_commands_size; semi_command_no++) {

            save_fds(&shell.fd0, &shell.fd1);

            int runs_bg = run_bg(shell.parser.semicolon_parsed_list[semi_command_no]);
            
            pipe_separation(&shell.parser, semi_command_no);
            shell.pipes.size = shell.parser.pipe_commands_size - 1;
            pipes_init(&shell.pipes);

            shell.curr_procs = malloc(sizeof(pid_t) * shell.parser.pipe_commands_size);

            // Loop through the "|" seperated tokens
            for (int pipe_command_no = 0; pipe_command_no < shell.parser.pipe_commands_size; pipe_command_no++) {

                get_arguments(&shell.parser, pipe_command_no);

                int alias_no;
                if ((alias_no = found_alias(shell.parser.arguments[0], &shell.aliases)) != -1) {
                    replace_alias(&shell.parser, &shell.aliases, alias_no);
                }
            
                handle_pipes(&shell.pipes, pipe_command_no);
                handle_redirections(&shell.parser, pipe_command_no);

                SysCmd sys_cmd;
                if ((sys_cmd = is_sys_cmd(shell.parser.arguments[0]))) {
                    exec_sys_cmd(sys_cmd, &shell);
                    // System commands don't get forked, shouldn't wait for them
                    shell.curr_procs[pipe_command_no] = 0;
                }

                if (!sys_cmd)
                    shell.curr_procs[pipe_command_no] = exec_user_cmd(&shell);

                if (runs_bg)
                    shell.bg_procs_num++;

                restore_fds(&shell.fd0, &shell.fd1);
            }

            if (!runs_bg)
                clean_fg_procs(shell.curr_procs, shell.parser.pipe_commands_size);

            destroy_pipes(&shell.pipes);
            // Clean up any background processes that finished their execution
            // Gets called in every iteration (non-blocking)
            clean_bg_procs(&shell.bg_procs_num);
            free(shell.curr_procs);
        }
    }
}