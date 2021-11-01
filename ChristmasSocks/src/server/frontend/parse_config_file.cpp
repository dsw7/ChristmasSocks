#include "parse_config_file.h"

bool read_configs_from_file(std::string path_config_file, Configs &configs)
{
    if (!file_exists(path_config_file))
    {
        std::cerr << "Path " + path_config_file + " does not exist!" << std::endl;
        return false;
    }

    std::string file_contents;
    read_file(path_config_file, file_contents);

    std::istringstream is_file(file_contents);
    std::string line;
    std::map<std::string, std::string> raw_configs;

    while (std::getline(is_file, line))  // we read line by line - up to \n in this case
    {
        std::istringstream is_line(line);

        std::size_t is_comment = line.find("#");

        if (is_comment != std::string::npos)
        {
            continue;
        }

        std::string key;

        // then for the extracted line, is_line, we read from the beginning of the line to =
        if (std::getline(is_line, key, '='))
        {

            std::string value;

            // then we read between = and \n
            if (std::getline(is_line, value))
            {
                raw_configs[key] = value;
            }
        }
    }

    std::map<std::string, std::string>::iterator it;
    for (it = raw_configs.begin(); it != raw_configs.end(); it++)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    return true;
}
