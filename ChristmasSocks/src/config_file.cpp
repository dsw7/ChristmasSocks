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
                this->raw_configs[key] = value;
            }
        }
    }
}

void ConfigParser::overwrite_default_configs_with_config_file_configs()
{
    if (!file_exists(this->path_config_file))
    {
        RootLogger::info("Could not find " + this->path_config_file);
        return;
    }

    RootLogger::info("Reading configurations from file " + this->path_config_file);
    read_file(this->path_config_file, this->file_contents);
    this->file_contents_to_raw_configs();

    std::map<std::string, std::string>::iterator it;

    for (it = this->raw_configs.begin(); it != this->raw_configs.end(); it++)
    {
        if (it->first.compare("tcp_port") == 0)
        {
            global_configs.tcp_port = std::stoi(it->second);
        }
        else if (it->first.compare("tcp_buffer_size") == 0)
        {
            global_configs.tcp_buffer_size = std::stoi(it->second);
        }
        else if (it->first.compare("max_num_connections_queue") == 0)
        {
            global_configs.max_num_connections_queue = std::stoi(it->second);
        }
        else
        {
            continue;
        }
    }
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
