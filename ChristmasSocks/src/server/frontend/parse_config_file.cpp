#include "parse_config_file.h"

bool read_configs_from_file(std::string path_config_file, Configs &configs)
{
    if (!read_file(path_config_file))
    {
        std::cerr << "Path " + path_config_file + " does not exist!" << std::endl;
        return false;
    }

    return true;
}
