#include "config_file.h"

void get_global_configs(std::map<std::string, std::string> &configs) {
    std::string file_contents;

    if (!read_file(CONFIG_FILEPATH, file_contents)) {
        RootLogger::info("Could not find " + CONFIG_FILEPATH + ". Using default configurations");
        return;
    }

    parse_configs(file_contents, configs);
    /*
     * The solution will be:
     * 1. Read in variables from config file if the file exists
     * 2. Load a struct with the configs
     * 3. If the particular configuration doesn't exist then default to using the extern from constants
     * 4. Pass the struct to class constructors
     */
}

// pass this struct into server and client classes
void set_global_configs(configs_t &global_configs) {
    global_configs.enable_debug_logging = ENABLE_DEBUG_LOGGING;  // this might need to go elsewhere
    global_configs.tcp_port = TCP_PORT;
    global_configs.max_num_connections_queue = MAX_NUM_CONNECTIONS_QUEUE;
    global_configs.tcp_buffer_size = TCP_BUFFER_SIZE;
}
