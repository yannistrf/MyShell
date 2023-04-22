#pragma once

// The struct that holds the information
// for the current pipes
typedef struct {

    int** table;    // the actual pipe descriptors
    int size;       // the number of the pipes
    
} Pipes;


void pipes_init(Pipes* pipes);
void destroy_pipes(Pipes* pipes);
void handle_pipes(Pipes* pipes, int proc_no);