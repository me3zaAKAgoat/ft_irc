#include "Commands.hpp"

bool	isValidNickChar(char c)
{
	return (!(c == '#' || c == ':' || c == ' '));
}

bool	isValidNick(std::string nickname)
{
	for (size_t i = 0; i < nickname.size(); i++)
	{
		if (!isValidNickChar(nickname[i]))
			return (false);
	}
	return (true);
}

bool	isAlreadyUsed(commandData& cmd, Server& server)
{
	std::map<int, Client *> clients = server.getClients();

	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
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
		Server::sendReply(ERR_NONICKNAMEGIVEN(client.getNickname()), client.getFd());
		return ;
	}
	if (!isValidNick(cmd.arguments[0]))
	{
		Server::sendReply(ERR_ERRONEUSNICKNAME(client.getNickname()), client.getFd());
		return ;
	}
	if (isAlreadyUsed(cmd, server))
	{
		Server::sendReply(ERR_NICKNAMEINUSE(client.getNickname()), client.getFd());
		return ;
	}
	if (!client.getNickname().empty())
	{
		std::string oldNick = client.getNickname();
		client.setNickname(cmd.arguments[0]);
		std::vector<Channel *>	channels = server.getChannels();
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i]->isMember(&client))
				channels[i]->broadcastMessage(&client, RPL_NICKCHANGE(oldNick, client.getNickname()));
		}
		Server::sendReply(RPL_NICKCHANGE(oldNick, client.getNickname()), client.getFd());
	}
	else
		client.setNickname(cmd.arguments[0]);
}