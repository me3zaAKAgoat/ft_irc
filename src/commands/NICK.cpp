#include "Commands.hpp"

bool	isValidCharNickname(char c)
{
	// check CHANTYPES also
	return (isalpha(c) || isdigit(c) || c == '[' || c == ']' || c == '\\' || c == '|'\
			|| c == '{' || c == '}' || c == ':' || c == '#');
}

bool	isValidNickname(commandData& cmd)
{
	std::string nickname = cmd.arguments[0];
	// if (isValidCharNickname(cmd[1][0]) && isdigit(cmd[1][0])) // whats the ref for this?
	// 	return (Server::sendResponse(": 432 " + client.getNickname() + " <nick> :Erroneous nickname\r\n", client.getFd()), false);
	for (size_t i = 0; i < nickname.size(); i++)
	{
		if (!isValidCharNickname(nickname[i]))
			return (false);
	}
	return (true);
}

bool	isAlreadyUsed(commandData& cmd, Server& server)
{
	std::cout << cmd.arguments[0] << std::endl;
	std::cout << server.getClients().begin()->second->getNickname() << std::endl;
	for (std::map<int, Client *>::iterator it = server.getClients().begin(); it != server.getClients().end(); ++it)
	{
		if (it->second->getNickname() == cmd.arguments[0])
			return (true);
	}
	return (false);
}

void	nickCmd(commandData& cmd, Server &server, Client& client)
{
	if (!client.isAuthenticated())
		return ;
	if (!cmd.arguments.size())
	{
		Server::sendResponse(": 431 " + client.getNickname() + " :No nickname given\r\n", client.getFd());
		return ;
	}
	if (!isValidNickname(cmd))
	{
		Server::sendResponse(": 432 " + client.getNickname() + " <nick> :Erroneous nickname\r\n", client.getFd());
		return ;
	}
	if (isAlreadyUsed(cmd, server))
	{
		Server::sendResponse(": 433 " + client.getNickname() + " <nick> :Nickname is already in use\r\n", client.getFd());
		return ;
	}
	client.setNickname(cmd.arguments[0]);
}