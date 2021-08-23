#ifndef COMMAND_LINE_INTERFACE_H
#define COMMAND_LINE_INTERFACE_H

#include <string>
#include <getopt.h>

struct Configs_cli
{
    bool         help;
    unsigned int tcp_port;
    int          tcp_buffer_size;
    std::string  bind_ip;
};

#endif
