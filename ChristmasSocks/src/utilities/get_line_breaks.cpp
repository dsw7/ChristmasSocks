#include "get_line_breaks.h"

std::string get_line_break(std::string &message)
{
    int size = message.size();

    if (message.compare(size - 2, 2, CHAR_END_OF_LINE) == 0)
    {
        return CHAR_END_OF_LINE;
    }

    std::string line_break;
    if (message.compare(size - 1, 1, CHAR_LINE_FEED) == 0)
    {
        line_break = CHAR_LINE_FEED;
    }
    else if (message.compare(size - 1, 1, CHAR_CARRIAGE_RETURN) == 0)
    {
        line_break = CHAR_CARRIAGE_RETURN;
    }
    else
    {
        line_break = "";
    }
    return line_break;
}
