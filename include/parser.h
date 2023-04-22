#pragma once

/*
    The struct that will parse the input of the user.
    Also holds important information about the parsed
    tokens.
*/
typedef struct {

    char* line;
    int line_size;
    char** semicolon_parsed_list;
    int semi_commands_size;
    char** pipe_parsed_list;
    int pipe_commands_size;
    char** arguments;
    int arg_size;
    
} CommandParser;

void parser_init(CommandParser* parser);
void parser_destroy(CommandParser* parser);
void semicolon_separation(CommandParser* parser);
void pipe_separation(CommandParser* parser, int command_no);
int get_arguments(CommandParser* parser, int pipe_no);
int handle_redirections(CommandParser* parser, int pipe_no);
int run_bg(char* command);
