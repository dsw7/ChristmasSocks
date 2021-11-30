#include "resolve_host.h"

// https://man7.org/linux/man-pages/man3/gethostbyname.3.html
bool resolve_host(std::string &host)
{
	hostent *struct_hostent = gethostbyname(host.c_str());

	if (struct_hostent == NULL)
	{
        RootLogger::info("Host " + host + " is unavailable");
        return false;
	}

	in_addr *address = (in_addr *)struct_hostent->h_addr;
    //std::string ip_address = inet_ntoa(*address);
    host = inet_ntoa(*address);

    return true;
}
