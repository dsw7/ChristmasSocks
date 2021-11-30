#include "resolve_master_host.h"

// https://man7.org/linux/man-pages/man3/gethostbyname.3.html
bool resolve_master_host(std::string &host)
{
    RootLogger::info("Attempting to resolve master host to IPv4 address");
	hostent *struct_hostent = gethostbyname(host.c_str());

	if (struct_hostent == NULL)
	{
        RootLogger::error("Host " + host + " is unavailable");
        return false;
	}

	in_addr *address = (in_addr *)struct_hostent->h_addr;
    host = inet_ntoa(*address);

    return true;
}
