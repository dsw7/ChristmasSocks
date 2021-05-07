#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <time.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

std::string get_current_datetime_string();
bool file_exists(std::string &pathname);
bool read_file(std::string &filepath, std::string &file_contents);

#endif
