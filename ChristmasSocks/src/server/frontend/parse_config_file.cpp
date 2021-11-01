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

    std::cout << file_contents << std::endl;

    return true;
}
