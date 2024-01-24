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