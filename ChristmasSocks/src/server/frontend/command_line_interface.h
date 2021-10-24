#ifndef COMMAND_LINE_INTERFACE_H
#define COMMAND_LINE_INTERFACE_H

#include <string>
#include <getopt.h>
#include "constants.h"
#include "utilities.h"
#include "help_messages.h"

struct Configs
{
    unsigned int tcp_port = TCP_PORT;
    unsigned int backlog = BACKLOG;
    int          tcp_buffer_size = TCP_BUFFER_SIZE;
    std::string  bind_ip = DEFAULT_BIND_IP;
    std::string  whitelist = DEFAULT_WHITELIST_IP;
};

void get_command_line_arguments(
    int argc, char **argv, Configs &configs
);

#endif
