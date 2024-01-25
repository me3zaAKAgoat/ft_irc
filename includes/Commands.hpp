#pragma once

#include "Irc.hpp"

void	nickCmd(std::vector<std::string> cmd, Client &client);
void	passCmd(Server& server, std::string cmd, Client &client, size_t numArgs);
void	userCmd(std::vector<std::string> cmd, Client &client);