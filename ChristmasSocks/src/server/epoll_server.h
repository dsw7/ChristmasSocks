#ifndef EPOLL_SERVER_H
#define EPOLL_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "logger.h"
#include "server_primitives.h"
#include "sysconfig_pipeline.h"
#include "incoming_client_primitives.h"
#include "constants.h"

class ServerImplMain: public ServerPrimitives, public IncomingClientPrimitives
{
    private:
        struct epoll_event ev;
        int epoll_fd = -1; // -1 means not set

    public:
        ServerImplMain(configs_t &configs): ServerPrimitives(
            configs.tcp_port, configs.max_num_connections_queue
        ),
        IncomingClientPrimitives(
            configs.tcp_buffer_size
        ) {}

        void incoming_client_setup();
        void server_setup();
        void open_epoll_file_descriptor();
        void register_server_fd_to_epoll_instance();
        void loop();
        void server_teardown();
};

#endif
