#pragma once

#include "Irc.hpp"

void	nick_cmd(std::vector<std::string> cmd, Client &client);
void	pass_cmd(Server& server, std::string cmd, Client &client, size_t numArgs);
void	user_cmd(std::vector<std::string> cmd, Client &client);