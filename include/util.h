#pragma once


#define PROMPT "$"
#define PATH_MAX 1024

void prompt(char* path, char* username);
int empty_line(char* line);
char* remove_spaces(char* str);
void free_list(char** list, int size);
int is_special_char(char* token);
