#include "config_file.h"

// This syntax is called an initializer list
ConfigParser::ConfigParser(std::string &path_config_file): path_config_file(path_config_file)
{
    this->global_configs.tcp_port = TCP_PORT;
    this->global_configs.max_num_connections_queue = MAX_NUM_CONNECTIONS_QUEUE;
    this->global_configs.tcp_buffer_size = TCP_BUFFER_SIZE;
}

void ConfigParser::file_contents_to_raw_configs()
{
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

void ConfigParser::overwrite_default_configs_with_config_file_configs()
{
    if (!file_exists(this->path_config_file)) {
        RootLogger::info("Could not find " + this->path_config_file);
        return;
    }

    RootLogger::info("Reading configurations from file " + this->path_config_file);
    read_file(this->path_config_file, this->file_contents);
    this->file_contents_to_raw_configs();

    // overwrite the config file stuff here
}

void ConfigParser::overwrite_configs_from_file_with_cli_options()
{
    // continue here
}

configs_t ConfigParser::load_configs()
{
    this->overwrite_default_configs_with_config_file_configs();
    this->overwrite_configs_from_file_with_cli_options();
    return this->global_configs;
}
