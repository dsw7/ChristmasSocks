#include "system_configurations.h"

SystemConfigurations::SystemConfigurations()
{
    this->configs.tcp_port = TCP_PORT;
    this->configs.max_num_connections_queue = MAX_NUM_CONNECTIONS_QUEUE;
    this->configs.tcp_buffer_size = TCP_BUFFER_SIZE;
    this->configs.handle_line_breaks = HANDLE_LINE_BREAKS;
    this->configs.bind_ip = DEFAULT_BIND_IP;
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
        else if (it->first.compare("bind_ip") == 0)
        {
            this->configs.bind_ip = it->second;
        }
        /*
         * Might add configs.handle_line_breaks handling here
         */
        else
        {
            continue;
        }
    }
}

configs_t SystemConfigurations::overwrite_config_file_configs_with_cli_args(int argc, char **argv)
{
    int option;

    while (1)
    {
        static struct option long_options[] =
        {
            {"port",               required_argument, 0, 'p'},
            {"buffer-size",        required_argument, 0, 'b'},
            {"bind-ip",            required_argument, 0, 'i'},
            {"handle-line-breaks", no_argument,       0, 'n'}
        };

        // What's the point of this?
        int option_index = 0;

        /* https://linux.die.net/man/3/getopt_long */
        option = getopt_long(
            argc, argv, "p:b:i:n", long_options, &option_index
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
            case 'i':
                this->configs.bind_ip = optarg;
                break;
            case 'n':
                this->configs.handle_line_breaks = true;
                break;
            default:
                help_message(argv[0]);
                exit(EXIT_FAILURE);
        }
    };

    return this->configs;
}
