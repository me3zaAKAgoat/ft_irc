#include "Commands.hpp"

void		userCmd(commandData& cmd, Client &client)
{
	if (!client.isAuthenticated())
	{
		return ;
	}
	if (client.getNickname().empty())
	{
		return ;
	}
	if (cmd.arguments.size() < 4)
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
		return ;
	}
	else if (!client.getUsername().empty())
	{
		Server::sendReply(ERR_ALREADYREGISTERED(client.getNickname()), client.getFd());
		return ;
	}
	if (cmd.arguments[1] != "0" || cmd.arguments[2] != "*")
	{
		Server::sendReply("Invalid arguments\n", client.getFd()); // change this message to a valid irc error reply
		return ;
	}
	client.setUsername(cmd.arguments[0]);
	client.setRealname(cmd.arguments[3]);
	client._register();
	Server::sendReply(RPL_WELCOME(client.getNickname()), client.getFd());
}
