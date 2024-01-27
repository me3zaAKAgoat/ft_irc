#pragma once

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>
#include <cstdlib>

#include <vector>
#include <map>

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"

#define COMMANDS_DELIMITER "\r\n"

std::vector<std::string>	split(const std::string &input, const std::string &separator = " ");
typedef typename std::vector<std::string>::iterator StringVectorIterator;
std::string					join(StringVectorIterator itBegin, StringVectorIterator itEnd, const std::string& separator);
