#ifndef RESOLVE_MASTER_HOST_H
#define RESOLVE_MASTER_HOST_H

#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "logger.h"
#include "configs.h"

bool resolve_master_host(Configs &configs);

#endif
