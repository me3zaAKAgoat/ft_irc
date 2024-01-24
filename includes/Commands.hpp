#pragma once

#include "Irc.hpp"
#include "Client.hpp"

void	NICK(std::vector<std::string> cmd, Client &client);
void	PASS(Server& server, std::string cmd, Client &client, size_t numArgs);
void	USER(std::vector<std::string> cmd, Client &client);