#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"
#include "pipes.h"

void prompt(char* path, char* username) {
    
    if (getcwd(path, PATH_MAX) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    printf("(%s): %s %s ", username, path, PROMPT);
}

int empty_line(char* line) {
    if (strlen(line) == 0)
        return 1;

    int i = 0;
    while (line[i] != '\0') {
        if (line[i] != ' ' && line[i] != '\t')
            return 0;
        i++;
    }

    return 1;
}

char* remove_spaces(char* str) {
    while(*str == ' ')
        str++;

    int end = strlen(str)-1;
    while (str[end] == ' ')
        end--;
    
    str[end+1] = '\0';
    return str;
}

void free_list(void** list, int size) {
    for (int i = 0; i < size; i++)
        free(list[i]);

    free(list);
}

int is_special_char(char* token) {
    return (!strcmp(token, ">") || !strcmp(token, ">>") || !strcmp(token, "<"));
}

void save_fds(int* fd0, int* fd1) {
    *fd0 = dup(0);
    *fd1 = dup(1);
}

void restore_fds(int* fd0, int* fd1) {
    dup2(*fd0, 0);
    dup2(*fd1, 1);
}

int exec_user_cmd(CommandParser* parser, int** pipes) {
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
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}