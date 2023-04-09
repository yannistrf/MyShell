#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

// #include <limits.h> // uncomment for PATH_MAX

#include "parse.h"

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

        getline(&line, &len, stdin);
        // Store the address so we can free later
        // bacuse strtok changes the string pointer
        line_to_free = line;
        int slen = strlen(line);


        // if user presses enter do nothing
        if (pressed_enter(line))
            continue;
        
        // Terminate string one char earlier, because of Enter
        line[slen-1] = '\0';
        
        char* command = strtok_r(line, ";", &line);
        char** arguments;
        char* token;
        while (command != NULL) {
            command = remove_spaces(command);

            token = strtok_r(command, " ", &command);
            int size = 2;   // program name and NULL
            arguments = malloc(sizeof(char*) * size);
            arguments[0] = token;

            while (token != NULL) {
                size++;
                arguments = realloc(arguments, sizeof(char*) * size);
                token = strtok_r(NULL, " ", &command);
                arguments[size-2] = token;
            }

            arguments[size-1] = NULL;

            // Check for built in commands
            if (!strcmp(arguments[0], "exit")) {
                printf("Exiting...\n");
                free(arguments);
                free(line_to_free);
                exit(EXIT_SUCCESS);
            }

            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            // Child process executes the command
            if (pid == 0)
                if (execvp(arguments[0], arguments) == -1) {
                    perror("mysh");
                    exit(EXIT_FAILURE);
                }

            // Wait for the child to return
            wait(NULL);

            free(arguments);
            command = strtok_r(NULL, ";", &line);
        }


        line = NULL;
        len = 0;
        free(line_to_free);
    }
}