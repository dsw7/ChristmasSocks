#ifndef CONFIGS_H
#define CONFIGS_H

#include <string>
#include "constants.h"

struct Configs
{
    unsigned int tcp_port = TCP_PORT;
    unsigned int backlog = BACKLOG;
    int          tcp_buffer_size = TCP_BUFFER_SIZE;
    std::string  bind_ip = DEFAULT_BIND_IP;
    std::string  master = DEFAULT_MASTER_IP;
};

#endif
