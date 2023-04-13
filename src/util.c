#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

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

void free_list(char** list, int size) {
    for (int i = 0; i < size; i++)
        free(list[i]);

    free(list);
}

int is_special_char(char* token) {
    // token = remove_spaces(token);
    return (!strcmp(token, ">") || !strcmp(token, ">>") || !strcmp(token, "&"));
}