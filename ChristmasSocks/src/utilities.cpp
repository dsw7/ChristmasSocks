#include "utilities.h"

std::string get_current_datetime_string() {
    time_t time_now = time(0);
    struct tm time_struct;
    char   time_buffer[80];

    time_struct = *localtime(&time_now);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%X", &time_struct);
    return time_buffer;
}

bool file_exists(std::string &pathname) {
    struct stat info;

    if (stat(pathname.c_str(), &info) != 0) {
        return false;
    }
    return true;
}
