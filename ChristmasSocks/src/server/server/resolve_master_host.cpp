#include "resolve_master_host.h"

// https://man7.org/linux/man-pages/man3/gethostbyname.3.html
bool resolve_master_host(Configs &configs)
{
    RootLogger::info("Attempting to resolve master host to IPv4 address");
    std::string incoming_host = configs.master;

	hostent *struct_hostent = gethostbyname(configs.master.c_str());
	if (struct_hostent == NULL)
	{
        RootLogger::error("Host " + configs.master + " is unavailable");
        return false;
	}

	in_addr *address = (in_addr *)struct_hostent->h_addr;
    configs.master = inet_ntoa(*address);

    RootLogger::info("Resolved '" + incoming_host + "' to '" + configs.master + "'");
    return true;
}
