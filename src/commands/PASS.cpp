#include "Commands.hpp"

void passCmd(commandData& cmd, Server& server, Client& client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
		return ;
	}
	if (client.isAuthenticated())
	{
		if (client.getNickname().empty())
			Server::sendReply(ERR_ALREADYREGISTERED(std::string("*")), client.getFd());
		else
			Server::sendReply(ERR_ALREADYREGISTERED(client.getNickname()), client.getFd());
		return ;
	}
	if (cmd.arguments[0] == server.getPassword())
		client.authenticate();
	else
	{
		if (client.getNickname().empty())
			Server::sendReply(ERR_PASSWDMISMATCH(std::string("*")), client.getFd());
		else
			Server::sendReply(ERR_PASSWDMISMATCH(client.getNickname()), client.getFd());
	}
}