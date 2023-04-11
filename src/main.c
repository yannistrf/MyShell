#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

// #include <limits.h> // uncomment for PATH_MAX

#include "parse.h"
// #include "util.h"

#define PROMPT "$"
#define PATH_MAX 1024

void prompt(char* path, char* username) {
    
    if (getcwd(path, PATH_MAX) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    printf("(%s): %s %s ", username, path, PROMPT);
}

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
        printf("# %s\n", parser.line);

        // Terminate string one char earlier, because of '\n'
        parser.line[parser.line_size] = '\0';

        if (empty_line(parser.line))
            break;
        
        semicolon_separation(&parser);
        for (int i = 0; i < parser.semi_size; i++) {
            printf("<%s>\n", parser.semicolon_parsed_list[i]);
            pipe_separation(&parser, i);
            for (int j = 0; j < parser.pipe_size; j++) {
                printf("*%s*\n", parser.pipe_parsed_list[j]);

                int arg_size;
                char** arguments = get_arguments(parser.pipe_parsed_list[j], &arg_size);
                arguments = realloc(arguments, sizeof(char*) * (arg_size+1));
                arguments[arg_size] = NULL;

                // check for sys commands

                // handle pipes

                // handle redirections

                pid_t pid = fork();
                if (pid == 0) {
                    if (execvp(arguments[0], arguments) == -1) {
                        perror("mysh");
                        exit(EXIT_FAILURE);
                    }  
                }

                free_list(arguments, arg_size);
                wait(NULL);
            }
        }

        
    }

    free(parser.line);
    free_list(parser.semicolon_parsed_list, parser.semi_size);
    free_list(parser.pipe_parsed_list, parser.pipe_size);
}