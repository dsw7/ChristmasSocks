#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <time.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>  // For ioctl() and TIOCGWINSZ
#include <unistd.h>     // For STDOUT_FILENO

std::string get_current_datetime_string();
bool file_exists(std::string &pathname);
void read_file(std::string &filepath, std::string &file_contents);
void render_separator();
void render_centered_text(const std::string &text);

#endif
