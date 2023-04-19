#pragma once

#include "parser.h"

#define PROMPT "$"
#define PATH_MAX 1024

void prompt(char* path, char* username);
int empty_line(char* line);
char* remove_spaces(char* str);
void free_list(void** list, int size);
int is_special_char(char* token);
void save_fds(int* fd0, int* fd1);
void restore_fds(int* fd0, int* fd1);
int exec_user_cmd(CommandParser* parser, int** pipes);
void clean_bg_procs(int* bg_procs);
