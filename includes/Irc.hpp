#pragma once

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>
#include <cstdlib>

#include <vector>
#include <map>

// Forward declaration
class Client;
class Server;

#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"

std::vector<std::string>	split(const std::string &input, const std::string &separator);
std::string 				join(std::vector<std::string>::iterator itBegin, std::vector<std::string>::iterator itEnd, std::string separator);
