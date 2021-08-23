#include "system_configurations.h"

void overwrite_root_configs_with_config_file_configs()
{
    if (!file_exists(CONFIG_FILEPATH))
    {
        RootLogger::info("Could not find " + CONFIG_FILEPATH);
        return;
    }

    RootLogger::info("Reading configurations from file " + CONFIG_FILEPATH);

    std::string file_contents;
    read_file(CONFIG_FILEPATH, file_contents);

    std::map<std::string, std::string> raw_configs;
    parse_config_file_contents(file_contents, raw_configs);

    std::map<std::string, std::string>::iterator it;

    for (it = raw_configs.begin(); it != raw_configs.end(); it++)
    {
        if (it->first.compare("tcp_port") == 0)
        {
            this->configs.tcp_port = std::stoi(it->second);
        }
        else if (it->first.compare("tcp_buffer_size") == 0)
        {
            this->configs.tcp_buffer_size = std::stoi(it->second);
        }
        else if (it->first.compare("max_num_connections_queue") == 0)
        {
            this->configs.max_num_connections_queue = std::stoi(it->second);
        }
        else if (it->first.compare("bind_ip") == 0)
        {
            this->configs.bind_ip = it->second;
        }
        else
        {
            continue;
        }
    }
}
