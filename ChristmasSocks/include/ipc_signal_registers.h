#ifndef IPC_SIGNAL_HANDLER_H
#define IPC_SIGNAL_HANDLER_H

#include "logger.h"
#include <csignal>

void sigint_handler(int signal);
void sigterm_handler(int signal);
void register_ipc_signals();

#endif
