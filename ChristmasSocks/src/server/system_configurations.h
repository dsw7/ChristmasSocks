#ifndef SYSTEM_CONFIGURATIONS_H
#define SYSTEM_CONFIGURATIONS_H

#include <map>
#include <getopt.h>

#include "utilities.h"
#include "constants.h"
#include "logger.h"
#include "help_menu.h"
#include "config_file_parser.h"

struct configs_t
{
    unsigned int tcp_port;
    unsigned int max_num_connections_queue;
    int          tcp_buffer_size;
    std::string  bind_ip;
};

class SystemConfigurations
{
    private:
        configs_t configs;

    public:
        SystemConfigurations();
        void overwrite_root_configs_with_config_file_configs();
        configs_t overwrite_config_file_configs_with_cli_args(int argc, char **argv);
};

#endif
