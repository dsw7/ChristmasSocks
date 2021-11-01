#ifndef PARSE_CONFIG_FILE_H
#define PARSE_CONFIG_FILE_H

#include <string>
#include <iostream>
#include "command_line_interface.h" // access Configs struct
#include "utilities.h"

bool read_configs_from_file(std::string path_config_file, Configs &configs);

#endif
