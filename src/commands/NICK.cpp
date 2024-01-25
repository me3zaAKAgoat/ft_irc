#include "Commands.hpp"

bool	isValidCharNickname(char c)
{
	// check CHANTYPES also
	if (isalpha(c) || isdigit(c) || c == '[' || c == ']' || c == '\\' || c == '|'\
			|| c == '{' || c == '}' || c == ':' || c == '#')
		return (true);
	return (false);
}

bool	isValidNickname(std::vector<std::string> cmd, Client &client)
{
	if (cmd.size() > 2)
		return (Server::sendResponse(": 432 " + client.getNickname() + " <nick> :Erroneous nickname\r\n", client.getFd()), false);
	else if (isValidCharNickname(cmd[1][0]) && isdigit(cmd[1][0]))
		return (Server::sendResponse(": 432 " + client.getNickname() + " <nick> :Erroneous nickname\r\n", client.getFd()), false);
	for (size_t i = 1; i < cmd[1].size(); i++)
	{
		if (!isValidCharNickname(cmd[1][i]))
			return (Server::sendResponse(": 432 " + client.getNickname() + " <nick> :Erroneous nickname\r\n", client.getFd()), false);
	}
	return (true);
}

bool	isAlreadyUsed(std::vector<std::string> cmd, Client &client)
{
	// doesn't works yet I needs to loop through the whole nicknames of clients and check
	(void)cmd;
	return (false);
	return (Server::sendResponse(": 433 " + client.getNickname() + " <nick> :Nickname is already in use\r\n", client.getFd()), true);
}

void	nickCmd(std::vector<std::string> cmd, Client &client)
{
	if (!client.isAuthenticated())
		return ;
	if (cmd.size() < 2)
	{
		Server::sendResponse(": 431 " + client.getNickname() + " :No nickname given\r\n", client.getFd());
		return ;
	}
	else if (!isValidNickname(cmd, client) || isAlreadyUsed(cmd, client))
		return ;
	if (!client.getNickname().empty())
		Server::sendResponse("; " + client.getNickname() + " changed his nickname to " + cmd[1] + ".\r\n", client.getFd()); // does not appears in lime chat (needs to modify)
	std::cout << client.getNickname() << " new Nickname: " << cmd[1] << std::endl; // debug
	client.setNickname(cmd[1]);
}