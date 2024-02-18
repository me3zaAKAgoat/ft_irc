#pragma once

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <netinet/in.h>
#include <cstdlib>
#include <arpa/inet.h>

#include <vector>
#include <map>
#include <set>

class Client;
class Server;
class Channel;


#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"
#include "response.hpp"


std::vector<std::string>	split(const std::string &input, const std::string &separator = " ");
typedef std::vector<std::string>::iterator StringVectorIterator;
std::string					join(std::vector<std::string> strings, const std::string& separator = " ");
std::string					join(StringVectorIterator itBegin, StringVectorIterator itEnd, const std::string& separator = " ");
bool						isValidArgs(int ac, const char *av[]);
bool						isValidPassword(const std::string password);
bool						isValidPort(const std::string port);
std::string					formulatePrefix(std::string hostname, std::string nickname, std::string username);