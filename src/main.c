#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

// #include <limits.h> // uncomment for PATH_MAX

#include "parse.h"
#include "built_in.h"

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
    
    char path[PATH_MAX];
    char* username;
    char* line = NULL;
    char* line_to_free;
    size_t len = 0;

    username = getlogin();

    while (1) {
        prompt(path, username);

        // getline allocates the neccessary space
        getline(&line, &len, stdin);

        // Store the address so we can free later
        // bacuse strtok changes the string pointer
        line_to_free = line;
        int slen = strlen(line);


        // if user presses enter do nothing
        if (pressed_enter(line))
            continue;
        
        // Terminate string one char earlier, because of '\n'
        line[slen-1] = '\0';

        int command_size;
        char** commands = semicolon_separation(line, &command_size);

        for (int command_no = 0; command_no < command_size; command_no++) {

            printf("| %s |\n", commands[command_no]);
            int no_of_pipes;
            char** programs = pipe_seperation(commands[command_no], &no_of_pipes);

            for (int program_no = 0; program_no < no_of_pipes; program_no++) {

                printf("| %s |\n", programs[program_no]);
                int arg_size;
                char** arguments = get_arguments(programs[program_no], &arg_size);
                arguments = realloc(arguments, sizeof(char*) * (arg_size+1));
                arguments[arg_size] = NULL;

                for (int i = 0; i < arg_size; i++)
                    printf("%s\n", arguments[i]);


                if (!strcmp(arguments[0], "exit")) {
                    printf("Exiting...\n");
                    free_list(arguments, arg_size);
                    free_list(programs, no_of_pipes);
                    free_list(commands, command_size);
                    free(line_to_free);
                    exit(EXIT_SUCCESS);
                }

                if (!strcmp(arguments[0], "cd")) {
                    if (arguments[2] != NULL) {
                        fprintf(stderr, "cd: too many arguments\n");
                        free_list(arguments, arg_size);
                        continue;
                    }

                    if (chdir(arguments[1]) == -1) {
                        perror("cd");
                    }
                    free_list(arguments, arg_size);
                    continue;
                }

                // Check fork fail
                pid_t pid = fork();
                if (pid == 0) {
                    if (execvp(arguments[0], arguments) == -1) {
                        perror("mysh");
                        exit(EXIT_FAILURE);
                    }                    
                }

                free_list(arguments, arg_size);
            }

            for (int program_no = 0; program_no < no_of_pipes; program_no++)
                wait(NULL);

            free_list(programs, no_of_pipes);
        }

        free_list(commands, command_size);
        line = NULL;
        len = 0;
        free(line_to_free);
    }
}