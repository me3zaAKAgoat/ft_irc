#pragma once

#include "Irc.hpp"
#include "Client.hpp"

void	nick(std::vector<std::string> cmd, Client &client);
void	pass(Server& server, std::string cmd, Client &client, size_t numArgs);
void	user(std::vector<std::string> cmd, Client &client);