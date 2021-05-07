#include "config_file.h"

ConfigParser::ConfigParser(std::string &path_config_file) {
    this->path_config_file = path_config_file;
}

void ConfigParser::file_contents_to_raw_configs() {
    std::istringstream is_file(this->file_contents);
    std::string line;

    while (std::getline(is_file, line)) {
        std::istringstream is_line(line);
        std::string key;

        if (std::getline(is_line, key, '=')) {
            std::string value;

            if (std::getline(is_line, value)) {
                this->raw_configs[key] = value;
            }
        }
    }
}

void ConfigParser::raw_configs_to_global_configs() {
    /*
     * now map raw_configs to global_configs struct here
     */

    this->global_configs.enable_debug_logging = ENABLE_DEBUG_LOGGING;  // this might need to go elsewhere
    this->global_configs.tcp_port = TCP_PORT;
    this->global_configs.max_num_connections_queue = MAX_NUM_CONNECTIONS_QUEUE;
    this->global_configs.tcp_buffer_size = TCP_BUFFER_SIZE;
}

void ConfigParser::extern_constants_to_global_configs() {
    this->global_configs.enable_debug_logging = ENABLE_DEBUG_LOGGING;  // this might need to go elsewhere
    this->global_configs.tcp_port = TCP_PORT;
    this->global_configs.max_num_connections_queue = MAX_NUM_CONNECTIONS_QUEUE;
    this->global_configs.tcp_buffer_size = TCP_BUFFER_SIZE;
}

configs_t ConfigParser::load_configs() {
    if (!read_file(this->path_config_file, this->file_contents)) {
        RootLogger::info("Could not find " + this->path_config_file + ". Using default configurations");
        this->extern_constants_to_global_configs();
    } else {
        RootLogger::info("Reading configurations from file " + this->path_config_file);
        this->file_contents_to_raw_configs();
        this->raw_configs_to_global_configs();
    }

    return this->global_configs;
}
