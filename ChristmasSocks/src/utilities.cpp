#include "utilities.h"

bool read_file(std::string &filepath, std::string &file_contents) {
    bool has_succeeded = true;

    std::ifstream filestream(filepath);

    if (filestream.fail()) {
        has_succeeded = false;
        return has_succeeded;
    }

    std::string line;
    while (getline (filestream, line)) {
        file_contents = line;
    }

    filestream.close();
    return has_succeeded;
}

std::string get_current_datetime_string() {
    time_t time_now = time(0);
    struct tm time_struct;
    char   time_buffer[80];

    time_struct = *localtime(&time_now);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%X", &time_struct);
    return time_buffer;
}

bool directory_exists(std::string &pathname) {
    bool is_valid_directory;
    struct stat info;

    if (stat(pathname.c_str(), &info) != 0) {
        Logger::error(pathname + " does not exist");
        is_valid_directory = false;
    } else if (info.st_mode & S_IFDIR) {
        // S_ISDIR() doesn't exist on my windows
        is_valid_directory = true;
    } else {
        Logger::error(pathname + " is not a directory");
        is_valid_directory = false;
    }

    return is_valid_directory;
}
