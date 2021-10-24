#ifndef COMMAND_LINE_INTERFACE_H
#define COMMAND_LINE_INTERFACE_H

#include <string>
#include <getopt.h>
#include <iostream>
#include "constants.h"
#include "utilities.h"

struct Configs
{
    unsigned int tcp_port = TCP_PORT;
    unsigned int backlog = BACKLOG;
    int          tcp_buffer_size = TCP_BUFFER_SIZE;
    std::string  bind_ip = DEFAULT_BIND_IP;
    std::string  whitelist = DEFAULT_WHITELIST_IP;
};

void help_general();
void help_usage(char *file);
void help_options();
void help_message(char *file);

void get_command_line_arguments(
    int argc, char **argv, Configs &configs
);

#endif
