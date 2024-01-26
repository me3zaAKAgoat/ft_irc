#pragma once

#include "Irc.hpp"

typedef struct
{
	std::string					prefix;
	std::string					name;
	std::vector<std::string>	arguments;
} commandData;

commandData	parseCommand(const std::string &requestMessage);
void		nickCmd(commandData& cmd, Server &server, Client& client);
void		passCmd(commandData& cmd, Client& client);
void		userCmd(commandData& cmd, Client &client);
