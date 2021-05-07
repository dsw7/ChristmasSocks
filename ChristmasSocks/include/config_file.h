#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <map>
#include "utilities.h"
#include "constants.h"
#include "logger.h"

struct configs_t {
    bool enable_debug_logging;
    unsigned int tcp_port;
    unsigned int max_num_connections_queue;
    unsigned int tcp_buffer_size;
};

class ConfigParser {
    private:
        std::map<std::string, std::string> raw_configs;

    public:
        ConfigParser();
        void get_global_configs(std::map<std::string, std::string> &configs);
        void set_global_configs(configs_t &configs);
}

#endif
