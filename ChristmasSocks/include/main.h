#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/epoll.h>

#include "logger.h"
#include "server.h"
#include "ipc_signal_registers.h"
#include "clients.h"
#include "config_file.h"

void server_impl_main(configs_t &configs);
void help_message(char *file);
bool lazy_run_help_messages(int argc, char **argv);

#endif
