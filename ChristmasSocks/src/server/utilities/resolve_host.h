#ifndef RESOLVE_HOST_H
#define RESOLVE_HOST_H

#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "logger,h"

bool resolve_host(std::string &host);

#endif
