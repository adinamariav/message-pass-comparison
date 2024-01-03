//
// Created by adina on 1/3/24.
//

#ifndef MESSAGE_PASS_COMPARISON_SIGNAL_H
#define MESSAGE_PASS_COMPARISON_SIGNAL_H

#include <linux/types.h>
#include <sys/signalfd.h>
#include <csignal>

#include "Connection.h"

class Signal : public Connection {
    struct signalfd_siginfo sigInfo;

public:
    Signal();
};


#endif //MESSAGE_PASS_COMPARISON_SIGNAL_H
