#include "utilities.h"

std::string get_current_datetime_string()
{
    time_t time_now = time(0);
    struct tm time_struct;
    char   time_buffer[80];

    time_struct = *localtime(&time_now);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%X", &time_struct);
    return time_buffer;
}

bool file_exists(std::string &filepath)
{
    struct stat info;

    if (stat(filepath.c_str(), &info) != 0)
    {
        return false;
    }
    return true;
}

void read_file(std::string &filepath, std::string &file_contents)
{
    std::ifstream filestream(filepath);

    std::string line;
    while (getline (filestream, line))
    {
        file_contents += line + "\n";
    }

    filestream.close();
}

void render_separator()
{
    struct winsize window_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);
    std::cout << std::string(window_size.ws_col, '=') << std::endl;
}
