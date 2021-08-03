#ifndef SYSCONFIG_PIPELINE_H
#define SYSCONFIG_PIPELINE_H

#include <map>
#include <sstream>
#include <getopt.h>

#include "utilities.h"
#include "constants.h"
#include "logger.h"
#include "help_menu.h"

struct configs_t
{
    unsigned int tcp_port;
    unsigned int max_num_connections_queue;
    unsigned int tcp_buffer_size;
    bool strip_line_breaks;
};

void parse_config_file_contents(
    std::string &file_contents, std::map<std::string, std::string> &raw_configs
);

class SystemParameters
{
    private:
        configs_t configs;

    public:
        SystemParameters();
        void overwrite_root_configs_with_config_file_configs();
        void overwrite_config_file_configs_with_cli_args(int argc, char **argv);
};

#endif
