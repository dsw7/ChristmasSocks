#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <time.h>
#include <sys/stat.h>

std::string get_current_datetime_string();
bool file_exists(std::string &pathname);

#endif
