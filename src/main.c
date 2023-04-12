#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

// #include <limits.h> // uncomment for PATH_MAX

#include "parser.h"
#include "sys_cmd.h"
#include "util.h"

#define READ 0
#define WRITE 1


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
        // printf("# %s\n", parser.line);

        // Terminate string one char earlier, because of '\n'
        parser.line[parser.line_size] = '\0';

        if (empty_line(parser.line))
            continue;
        
        semicolon_separation(&parser);
        for (int i = 0; i < parser.semi_size; i++) {
            // printf("<%s>\n", parser.semicolon_parsed_list[i]);
            pipe_separation(&parser, i);
            // int* pipes[2] = malloc(sizeof(int[2]) * (parser.pipe_size-1));
            // for (int pipe_no = 0; pipe_no < parser.pipe_size-1; pipe_no++)
            //     pipe(pipes[pipe_no]);
            for (int j = 0; j < parser.pipe_size; j++) {
                // printf("*%s*\n", parser.pipe_parsed_list[j]);

                get_arguments(&parser, j);

                // handle pipes
                // if (pipes != NULL) {
                //     if (j != 0) {

                //     }
                // }

                // handle redirections

                SysCmd sys_cmd;
                if ((sys_cmd = is_sys_cmd(parser.arguments[0]))) {
                    exec_sys_cmd(sys_cmd, &parser);
                    continue;
                }

                pid_t pid = fork();
                if (pid == 0) {
                    if (execvp(parser.arguments[0], parser.arguments) == -1) {
                        perror("mysh");
                        parser_destroy(&parser);
                        exit(EXIT_FAILURE);
                    }  
                }

                wait(NULL);
            }
        }
    }
}