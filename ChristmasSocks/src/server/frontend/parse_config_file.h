#ifndef PARSE_CONFIG_FILE_H
#define PARSE_CONFIG_FILE_H

#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include "command_line_interface.h" // access Configs struct
#include "utilities.h"

bool read_configs_from_file(std::string path_config_file, Configs &configs);
void parse_configs(std::string &file_contents, std::map<std::string, std::string> &raw_configs);

#endif
