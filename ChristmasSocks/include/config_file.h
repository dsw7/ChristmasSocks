#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include "utilities.h"
#include "constants.h"
#include "logger.h"

struct configs_t {
    bool enable_debug_logging;
    unsigned int tcp_port;
    unsigned int max_num_connections_queue;
    unsigned int tcp_buffer_size;
};

void read_config_file();
void set_global_configs(configs_t &configs);

#endif
