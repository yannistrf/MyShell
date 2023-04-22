#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "util.h"
#include "pipes.h"

void prompt(char* path, char* username) {

    if (getcwd(path, PATH_MAX) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    printf("(%s): %s %s ", username, path, PROMPT);
}

// Returns 1 if the line is empty, 0 otherwise
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

// Removes spaces at the start and at the end of the string
char* remove_spaces(char* str) {
    while(*str == ' ')
        str++;

    int end = strlen(str)-1;
    while (str[end] == ' ')
        end--;
    
    str[end+1] = '\0';
    return str;
}

// Can free any 2d array
void free_list(void** list, int size) {
    for (int i = 0; i < size; i++)
        free(list[i]);

    free(list);
}

int is_special_char(char* token) {
    return (
            !strcmp(token, ">") ||
            !strcmp(token, ">>") ||
            !strcmp(token, "<") ||
            !strcmp(token, "&")
        );
}

void save_fds(int* fd0, int* fd1) {
    *fd0 = dup(0);
    *fd1 = dup(1);
}

void restore_fds(int* fd0, int* fd1) {
    dup2(*fd0, 0);
    dup2(*fd1, 1);
}
