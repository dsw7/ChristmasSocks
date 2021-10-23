#ifndef IPC_SIGNAL_HANDLER_H
#define IPC_SIGNAL_HANDLER_H

#include <csignal>
#include <unistd.h>
#include "logger.h"
#include "header_footer.h"

void sigint_handler(int signal);
void sigterm_handler(int signal);
void register_ipc_signals();

#endif
