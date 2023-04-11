#pragma once

#include "util.h"


char* remove_spaces(char*);
int empty_line(char* line);
void semicolon_separation(CommandParser* parser);
void pipe_separation(CommandParser* parser, int command_no);
char** get_arguments(char*, int*);