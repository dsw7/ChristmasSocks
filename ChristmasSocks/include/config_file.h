#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <map>
#include <sstream>
#include "utilities.h"
#include "constants.h"
#include "logger.h"

struct configs_t
{
    bool enable_debug_logging;
    unsigned int tcp_port;
    unsigned int max_num_connections_queue;
    unsigned int tcp_buffer_size;
};

class ConfigParser
{
    private:
        std::string path_config_file;
        std::map<std::string, std::string> raw_configs;
        std::string file_contents;
        configs_t global_configs;

    public:
        ConfigParser(std::string &path_config_file);
        void file_contents_to_raw_configs();
        void overwrite_default_configs_with_config_file_configs();
        void overwrite_configs_from_file_with_cli_options();
        configs_t load_configs();
};

#endif
