#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Irc.hpp"
#include "Client.hpp"

void	NICK(std::vector<string> cmd, Client &newClient);
void	PASS(std::vector<string> cmd, Client &newClient);
void	USER(std::vector<string> cmd, Client &newClient);

#endif