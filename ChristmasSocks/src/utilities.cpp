#include "utilities.h"

std::string get_current_datetime_string() {
    time_t time_now = time(0);
    struct tm time_struct;
    char   time_buffer[80];

    time_struct = *localtime(&time_now);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%X", &time_struct);
    return time_buffer;
}

bool file_exists(std::string &filepath) {
    struct stat info;

    if (stat(filepath.c_str(), &info) != 0) {
        return false;
    }
    return true;
}

bool read_file(std::string &filepath, std::string &file_contents) {
    std::ifstream filestream(filepath);

    if (!file_exists(filepath)) {
        return false;
    }

    std::string line;
    while (getline (filestream, line)) {
        file_contents = line;
    }

    filestream.close();
    return true;
}

void parse_configs(std::string &file_contents, std::map<std::string, std::string> &configs) {
    std::istringstream is_file(file_contents);
    std::string line;

    while (std::getline(is_file, line)) {
        std::istringstream is_line(line);
        std::string key;

        if (std::getline(is_line, key, '=')) {
            std::string value;

            if (std::getline(is_line, value)) {
                configs[key] = value;
            }
        }
    }
}
