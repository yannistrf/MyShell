#include <stdlib.h>
#include <unistd.h>

#include "shell.h"

void shell_init(MyShell* sh) {
    sh->username = getlogin();
    parser_init(&sh->parser);
    aliases_init(&sh->aliases);
    sh->bg_procs_num = 0;
    history_init(&sh->history);
}

void shell_destroy(MyShell* sh) {
    parser_destroy(&sh->parser);
    destroy_pipes(&sh->pipes);
    aliases_destroy(&sh->aliases);
    free(sh->curr_procs);
    history_destroy(&sh->history);
}
