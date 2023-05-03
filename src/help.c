#include <stdio.h>

#include "help.h"

char* cmd_desc[] = {
    "> exit\nExits program properly, freeing all occupied resources\n",
    "> help\nPrints a short description for all of the built in commands\n",
    "> cd <path>\nNavigate to the pointed directory\n",
    "> history\nPrints the most recent commands\n",
    "> createalias <alias> \"<command>\"\nCreates an alias that gets stored in the memory and can later be used to execute the given command\n",
    "> destroyalias <alias>\nDeletes the given alias and can no longer be used\n",
    "> printaliases\nPrints all alias entries\n",
};


void sys_help(CommandParser* parser) {
    
    if (parser->arg_size > 1) {
        fprintf(stderr, "help: too many arguments\n");
        return;
    }

    int size = sizeof(cmd_desc) / sizeof(char*);

    printf("\n");
    for (int i = 0; i < size; i++)
        printf("%s\n", cmd_desc[i]);
}