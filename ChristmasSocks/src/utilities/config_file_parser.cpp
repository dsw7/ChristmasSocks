#include "config_file_parser.h"

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
