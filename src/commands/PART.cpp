#include "Irc.hpp"

void	partCmd(commandData& cmd, Server& server, Client* client)
{
	std::vector<Channel *> channels = server.getChannels();
	bool	channelExists; 

	if (!cmd.arguments.size())
	{
		Server::sendReply(": 461 " + client->getNickname() + " PART :Not enough parameters\r\n", client->getFd());
		return ;
	}
	for (size_t i = 0; i < cmd.arguments.size(); i++)
	{
		channelExists = false; 
		if (cmd.arguments[i][0] != '#' && cmd.arguments[i][0] != '&')
		{
			Server::sendReply(": 403 " + client->getNickname() + " " + cmd.arguments[i] + " :No such channel\r\n", client->getFd());
			continue ;
		}
		for (size_t j = 0; j < channels.size(); j++)
		{
			if (channels[j]->getName() == cmd.arguments[i])
			{
				channelExists = true;
				if (!channels[j]->isMember(client))
				{
					Server::sendReply(": 442 " + client->getNickname() + " " + cmd.arguments[i] + " :You're not on that channel\r\n", client->getFd());
					break ;
				}
				channels[j]->removeMember(client);
				if (!channels[j]->getMembers().size())
					server.removeChannel(channels[j]);
				break ;
			}
		}
		if (!channelExists)
			Server::sendReply(": 403 " + client->getNickname() + " " + cmd.arguments[i] + " :No such channel\r\n", client->getFd());
	}
}