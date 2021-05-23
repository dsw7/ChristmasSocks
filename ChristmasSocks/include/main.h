#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include "logger.h"
#include "server.h"
#include "ipc_signal_registers.h"
#include "clients.h"
#include "sysconfig_pipeline.h"

void server_impl_main(configs_t &configs);

#endif
