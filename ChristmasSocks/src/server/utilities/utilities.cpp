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
    window_size.ws_col = 0; // handle 'Conditional jump or move depends on uninitialized value(s)'

    // https://man7.org/linux/man-pages/man2/ioctl.2.html
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size) == 0)
    {
        std::cout << std::string(window_size.ws_col, '=') << std::endl;
    }
    else
    {
        // Render separator to some dummy length if output is redirected (i.e. inappropriate ioctl for device)
        std::cout << std::string(20, '=') << std::endl;
    }
}

void render_centered_text(const std::string &text)
{
    struct winsize window_size;
    window_size.ws_col = 0;
    int offset;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size) == 0)
    {
        offset = (window_size.ws_col / 2) - (text.size() / 2);
    }
    else
    {
        offset = 1;
    }

    std::cout << std::string(offset, ' ');
    std::cout << text << std::endl;
}
