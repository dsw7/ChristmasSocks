#ifndef MAIN_H
#define MAIN_H

#include <sys/epoll.h>
#include "logger.h"
#include "server.h"
#include "ipc_signal_registers.h"
#include "clients.h"
#include "config_file.h"

void server_impl_main();

#endif
