#include "config_file.h"

void set_root_configs(configs_t &configs)
{
    configs.tcp_port = TCP_PORT;
    configs.max_num_connections_queue = MAX_NUM_CONNECTIONS_QUEUE;
    configs.tcp_buffer_size = TCP_BUFFER_SIZE;
}

void parse_config_file_contents(std::string &file_contents, std::map<std::string, std::string> &raw_configs)
{
    std::istringstream is_file(file_contents);
    std::string line;

    while (std::getline(is_file, line))
    {
        std::istringstream is_line(line);

        std::size_t comment = line.find("#");
        if (comment != std::string::npos)
        {
            // std::string::npos returned if string does not contain #
            continue;
        }

        std::string key;

        if (std::getline(is_line, key, '='))
        {
            std::string value;

            if (std::getline(is_line, value))
            {
                raw_configs[key] = value;
            }
        }
    }
}

void overwrite_default_configs_with_config_file_configs(configs_t &configs, std::string &path_config_file)
{
    if (!file_exists(path_config_file))
    {
        RootLogger::info("Could not find " + path_config_file);
        return;
    }

    RootLogger::info("Reading configurations from file " + path_config_file);

    std::string file_contents;
    read_file(path_config_file, file_contents);

    std::map<std::string, std::string> raw_configs;
    parse_config_file_contents(file_contents, raw_configs);

    std::map<std::string, std::string>::iterator it;

    for (it = raw_configs.begin(); it != raw_configs.end(); it++)
    {
        if (it->first.compare("tcp_port") == 0)
        {
            configs.tcp_port = std::stoi(it->second);
        }
        else if (it->first.compare("tcp_buffer_size") == 0)
        {
            configs.tcp_buffer_size = std::stoi(it->second);
        }
        else if (it->first.compare("max_num_connections_queue") == 0)
        {
            configs.max_num_connections_queue = std::stoi(it->second);
        }
        else
        {
            continue;
        }
    }
}
