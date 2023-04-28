#pragma once

#define PROMPT "$"
#define PATH_MAX 1024

void prompt(char* path, char* username);
int empty_line(char* line);
char* remove_spaces(char* str);
void free_list(void** list, int size);
int is_special_char(char* token);
void save_fds(int* fd0, int* fd1);
void restore_fds(int* fd0, int* fd1);
