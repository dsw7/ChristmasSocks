#ifndef COMMAND_LINE_INTERFACE_H
#define COMMAND_LINE_INTERFACE_H

#include <string>
#include <getopt.h>
#include <iostream>
#include "constants.h"

struct Configs
{
    unsigned int tcp_port = TCP_PORT;
    unsigned int max_num_connections_queue = MAX_NUM_CONNECTIONS_QUEUE;
    int          tcp_buffer_size = TCP_BUFFER_SIZE;
    std::string  bind_ip = DEFAULT_BIND_IP;
};

void help_message(
    char *file
);

void get_command_line_arguments(
    int argc, char **argv, Configs &configs
);

#endif
