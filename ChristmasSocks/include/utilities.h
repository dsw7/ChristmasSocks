#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <time.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <map>

std::string get_current_datetime_string();
bool file_exists(std::string &pathname);
bool read_file(std::string &filepath, std::string &file_contents);
void parse_configs(std::string &file_contents, std::map<std::string, std::string> &configs);

#endif
