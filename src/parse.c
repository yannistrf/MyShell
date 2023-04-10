#include <string.h>
#include <stdlib.h>

int pressed_enter(char* line) {
    if (strlen(line) == 1)
        return 1;

    return 0;
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

char** semicolon_separation(char* line, int* size) {
    char* token = strtok_r(line, ";", &line);
    *size = 0;

    char** token_list = NULL;

    while (token != NULL) {
        token = remove_spaces(token);
        (*size)++;
        token_list = realloc(token_list, sizeof(char*) * (*size));
        token_list[*size-1] = malloc(strlen(token)+1);
        strcpy(token_list[*size-1], token);
        token = strtok_r(NULL, ";", &line);
    }

    return token_list;
}

char** pipe_seperation(char* command, int* size) {
    char* token = strtok_r(command, "|", &command);
    *size = 0;

    char** token_list = NULL;

    while (token != NULL) {
        token = remove_spaces(token);
        (*size)++;
        token_list = realloc(token_list, sizeof(char*) * (*size));
        token_list[*size-1] = malloc(strlen(token)+1);
        strcpy(token_list[*size-1], token);
        token = strtok_r(NULL, "|", &command);
    }

    return token_list;
}

void free_list(char** list, int size) {
    for (int i = 0; i < size; i++)
        free(list[i]);

    free(list);
}

int is_special_char(char* token) {
    token = remove_spaces(token);
    return !strcmp(token, ">") || !strcmp(token, ">>") || !strcmp(token, "&");
}

char** get_arguments(char* program, int* size) {
    char* token = strtok_r(program, " ", &program);
    *size = 0;

    char** token_list = NULL;

    while (token != NULL) {
        token = remove_spaces(token);
        if (is_special_char(token))
            break;
        (*size)++;
        token_list = realloc(token_list, sizeof(char*) * (*size));
        token_list[*size-1] = malloc(strlen(token)+1);
        strcpy(token_list[*size-1], token);
        token = strtok_r(NULL, " ", &program);
    }

    return token_list;   
}