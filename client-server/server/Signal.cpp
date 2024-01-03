//
// Created by adina on 1/3/24.
//

#include "Signal.h"

Signal::Signal() {
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);                /* CTRL-C */
    sigaddset(&mask, SIGQUIT);               /* CTRL-\ */

    sigprocmask(SIG_BLOCK, &mask, NULL);
    this->descriptor = signalfd(-1, &mask, 0);
    this->id = -1;
}
