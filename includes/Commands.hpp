#pragma once

#include "Irc.hpp"
#include "Client.hpp"

void	NICK(std::vector<std::string> cmd, Client &client);
void	PASS(std::vector<std::string> cmd, Client &client);
void	USER(std::vector<std::string> cmd, Client &client);