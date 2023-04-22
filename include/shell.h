#pragma once

#include "util.h"
#include "parser.h"
#include "pipes.h"
#include "alias.h"

/*
    The struct that holds all of the
    information for the current state
    of program.
*/
typedef struct {

    char* username;
    char path[PATH_MAX];    // The working direcotry path, updated each time
    CommandParser parser;
    Aliases aliases;
    int fd0;                // The initial descriptor for stdout
    int fd1;                // The initial descriptor for stdin
    Pipes pipes;
    int* curr_procs;        // A table with the pids of the processes running
    int bg_procs_num;       // How many process are running in the background
    
} MyShell;


void shell_init(MyShell* sh);
void shell_destroy(MyShell* sh);
