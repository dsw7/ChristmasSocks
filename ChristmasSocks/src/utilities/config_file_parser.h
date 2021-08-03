#ifndef CONFIG_FILE_PARSER_H
#define CONFIG_FILE_PARSER_H

#include <map>
#include <sstream>
#include <string>

void parse_config_file_contents(
    std::string &file_contents, std::map<std::string, std::string> &raw_configs
);

#endif
