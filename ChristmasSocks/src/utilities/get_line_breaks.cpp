#include "get_line_breaks.h"

std::string get_line_break(std::string &message)
{
    int size = message.size();

    if (message.compare(size - 2, 2, "\r\n") == 0)
    {
        return "\r\n";
    }

    std::string line_break;
    if (message.compare(size - 1, 1, "\n") == 0)
    {
        line_break = "\n";
    }
    else if (message.compare(size - 1, 1, "\r") == 0)
    {
        line_break = "\r";
    }
    else
    {
        line_break = "";
    }
    return line_break;
}
