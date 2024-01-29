#include "Irc.hpp"

/* handle commas aka multiple channels */
void	partCmd(commandData& cmd, Server& server, Client* client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), cmd.name), client->getFd());
		return ;
	}
	std::vector<std::string> paramChannels = split(cmd.arguments[0], ",");

	for (size_t i = 0; i < paramChannels.size(); i++)
	{
		if (Channel::isValidChannelName(cmd.arguments[i]))
		{
			Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[0]), client->getFd());
			continue ;
		}
		Channel *channel = server.getChannelByName(cmd.arguments[i]);
		if (!channel)
		{
			Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[i]), client->getFd());
			continue ;
		}
		if (!channel->isMember(client))
		{
			Server::sendReply(ERR_NOTONCHANNEL(client->getNickname(), cmd.arguments[i]), client->getFd());
			continue ;
		}
		channel->removeMember(server, client);
	}
}