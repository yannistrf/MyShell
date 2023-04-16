#pragma once

int** pipes_init(int num_of_pipes);
void destroy_pipes(int** pipes, int size);
void handle_pipes(int** pipes, int piped_proc_no, int num_of_pipes);