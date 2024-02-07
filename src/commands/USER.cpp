#include "Commands.hpp"

void		userCmd(commandData& cmd, Server &server, Client &client)
{
	(void)server;
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
	client.setUsername(cmd.arguments[0]);
	client.setRealname(cmd.arguments[3]);
	client._register();
	if (client.getNickname() != "BMOBot")
	{
		Server::sendReply(RPL_WELCOME(client.getNickname()), client.getFd());
		Client *bot = server.getClientByNickname("BMOBot");
		if (bot)
			Server::sendReply(("CLIENT " + client.getNickname()), bot->getFd());
	}
}
