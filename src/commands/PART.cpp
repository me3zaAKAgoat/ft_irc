#include "Irc.hpp"

void	partCmd(commandData& cmd, Server& server, Client* client)
{
	std::vector<Channel *> channels = server.getChannels();
	bool	channelExists; 

	if (!cmd.arguments.size())
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), cmd.name), client->getFd());
		return ;
	}
	for (size_t i = 0; i < cmd.arguments.size(); i++)
	{
		channelExists = false; 
		if (cmd.arguments[i][0] != '#' && cmd.arguments[i][0] != '&')
		{
			Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[0]), client->getFd());
			continue ;
		}
		for (size_t j = 0; j < channels.size(); j++)
		{
			if (channels[j]->getName() == cmd.arguments[i])
			{
				channelExists = true;
				if (!channels[j]->isMember(client))
				{
					Server::sendReply(ERR_NOTONCHANNEL(client->getNickname(), cmd.arguments[i]), client->getFd());
					break ;
				}
				channels[j]->removeMember(client);
				if (!channels[j]->getMembers().size())
					server.removeChannel(channels[j]);
				break ;
			}
		}
		if (!channelExists)
			Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[i]), client->getFd());
	}
}