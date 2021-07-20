#ifndef EPOLL_SERVER_H
#define EPOLL_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include "logger.h"
#include "server_primitives.h"
#include "incoming_client_primitives.h"

void server_impl_main(configs_t &configs);

#endif