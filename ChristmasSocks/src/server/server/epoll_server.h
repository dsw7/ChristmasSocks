#ifndef EPOLL_SERVER_H
#define EPOLL_SERVER_H

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <sys/epoll.h>

#include "logger.h"
#include "server_primitives.h"
#include "incoming_client_primitives.h"
#include "constants.h"
#include "command_help.h"
#include "command_sleep.h"

class ServerImplMain: public ServerPrimitives, public IncomingClientPrimitives
{
    private:
        struct epoll_event ev;
        int epoll_fd = -1; // -1 means not set
        std::string message;
        bool exit_loop = false;

    public:
        ServerImplMain(Configs &configs): ServerPrimitives(
            configs
        ),
        IncomingClientPrimitives(
            configs
        ) {}

        void incoming_client_setup();
        void server_setup();
        void open_epoll_file_descriptor();
        void register_server_fd_to_epoll_instance();
        void handle_incoming_message();
        void loop();
        void server_teardown();
};

#endif
