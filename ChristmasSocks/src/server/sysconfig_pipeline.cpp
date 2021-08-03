#include "sysconfig_pipeline.h"

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

SystemConfigurations::SystemConfigurations()
{
    this->configs.tcp_port = TCP_PORT;
    this->configs.max_num_connections_queue = MAX_NUM_CONNECTIONS_QUEUE;
    this->configs.tcp_buffer_size = TCP_BUFFER_SIZE;
    this->configs.strip_line_breaks = STRIP_LINE_BREAK;
}

void SystemConfigurations::overwrite_root_configs_with_config_file_configs()
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
        /*
         * Might add configs.strip_line_breaks handling here
         */
        else
        {
            continue;
        }
    }
}

void SystemConfigurations::overwrite_config_file_configs_with_cli_args(int argc, char **argv)
{
    int option;

    while (1)
    {
        static struct option long_options[] =
        {
            {"port",              required_argument, 0, 'p'},
            {"buffer-size",       required_argument, 0, 'b'},
            {"strip-line-breaks", no_argument,       0, 'n'}
        };

        // What's the point of this?
        int option_index = 0;

        /* https://linux.die.net/man/3/getopt_long */
        option = getopt_long(
            argc, argv, "p:b:n", long_options, &option_index
        );

        // End of options
        if (option == -1)
        {
            break;
        }

        switch (option)
        {
            case 'p':
                this->configs.tcp_port = atoi(optarg);
                break;
            case 'b':
                this->configs.tcp_buffer_size = atoi(optarg);
                break;
            case 'n':
                this->configs.strip_line_breaks = true;
                break;
            default:
                help_message(argv[0]);
                exit(EXIT_FAILURE);
        }
    };
}
