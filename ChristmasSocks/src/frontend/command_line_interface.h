#ifndef COMMAND_LINE_INTERFACE_H
#define COMMAND_LINE_INTERFACE_H

#include <string>
#include <getopt.h>
#include <iostream>
#include "help_menu.h"

struct Configs_cli
{
    unsigned int tcp_port;
    int          tcp_buffer_size;
    std::string  bind_ip;
};

void help_message(
    char *file
);

void get_command_line_arguments(
    int argc, char **argv, Configs_cli &configs
);

#endif
