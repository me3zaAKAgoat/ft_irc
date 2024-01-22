#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>

// Forward declaration
class Client;
class Server;

#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"