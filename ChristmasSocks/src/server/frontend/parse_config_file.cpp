#include "parse_config_file.h"

bool read_configs_from_file(int argc, char **argv)
{
    if (argc < 2)
    {
        RootLogger::info("No arguments provided. Using system defaults");
        return false;
    }

    return true;
}
