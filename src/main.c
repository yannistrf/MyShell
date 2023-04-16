#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#include "parser.h"
#include "sys_cmd.h"
#include "util.h"
#include "pipes.h"


int main() {
    char* username = getlogin();
    char path[PATH_MAX];
    
    CommandParser parser;
    parser_init(&parser);

    size_t buf_size = 0;

    while (1) {

        prompt(path, username);
        
        // Allocates space for line
        parser.line_size = getline(&parser.line, &buf_size, stdin) - 1;

        // Terminate string one char earlier, because of '\n'
        parser.line[parser.line_size] = '\0';

        if (empty_line(parser.line))
            continue;
        
        semicolon_separation(&parser);
        for (int semi_command_no = 0; semi_command_no < parser.semi_commands_size; semi_command_no++) {

            int fd0, fd1;
            save_fds(&fd0, &fd1);

            pipe_separation(&parser, semi_command_no);
            int num_of_pipes = parser.pipe_commands_size-1;
            int** pipes =  pipes_init(num_of_pipes);

            for (int pipe_command_no = 0; pipe_command_no < parser.pipe_commands_size; pipe_command_no++) {

                get_arguments(&parser, pipe_command_no);

                if (sizeof(pipes) != 0)
                    handle_pipes(pipes, pipe_command_no, num_of_pipes);

                handle_redirections(&parser, pipe_command_no);

                SysCmd sys_cmd;
                if ((sys_cmd = is_sys_cmd(parser.arguments[0])))
                    exec_sys_cmd(sys_cmd, &parser, pipes);

                if (!sys_cmd)
                    exec_user_cmd(&parser, pipes);

                restore_fds(&fd0, &fd1);
            }

            for (int pipe_command_no = 0; pipe_command_no < parser.pipe_commands_size; pipe_command_no++)
                wait(NULL);

            destroy_pipes(pipes, num_of_pipes);
        }
    }
}