#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <map>
#include <sstream>
#include <getopt.h>

#include "utilities.h"
#include "constants.h"
#include "logger.h"

struct configs_t
{
    unsigned int tcp_port;
    unsigned int max_num_connections_queue;
    unsigned int tcp_buffer_size;
};

void help_message(
    char *file
);
bool help_message_lazy_runner(
    int argc, char **argv
);
void parse_config_file_contents(
    std::string &file_contents, std::map<std::string, std::string> &raw_configs
);
void set_root_configs(
    configs_t &configs
);
void overwrite_root_configs_with_config_file_configs(
    configs_t &configs, std::string &path_config_file
);
void overwrite_config_file_configs_with_cli_args(
    configs_t &global_configs, int argc, char **argv
);

#endif