#include <stdlib.h>
#include <signal.h>

#include "sig.h"


void sig_handle_parent() {
    struct sigaction act = {0}; // use universal initializer because valgrind complaining
    act.sa_handler = SIG_IGN;
    sigfillset(&(act.sa_mask));
    sigaction (SIGINT, &act, NULL);
    sigaction (SIGTSTP, &act, NULL);
}

void sig_handle_child() {
    struct sigaction act = {0}; // use universal initializer because valgrind complaining
    act.sa_handler = SIG_DFL;
    sigfillset(&(act.sa_mask));
    sigaction (SIGINT, &act, NULL);
    sigaction (SIGTSTP, &act, NULL);
}