#include "Irc.hpp"


/* 
there's more stuff that hasnt been implemented yet

- send commands that the server receives which affect the channel
- RPL_TOPIC
- RPL_NAMREPLY
- add channel limit management
https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
*/
void	joinCmd(commandData& cmd, Server& server, Client* client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), cmd.name), client->getFd());
		return ;
	}
	std::vector<Channel *> channels = server.getChannels();
	bool	channelExists; 

	std::vector<std::string> paramChannels = split(cmd.arguments[0], ",");
	std::vector<std::string> paramKeys;
	if (cmd.arguments.size() > 1)
		paramKeys = split(cmd.arguments[1], ",");
	for (size_t i = 0; i < paramChannels.size(); i++)
	{
		channelExists = false; 
		if (paramChannels[i][0] != '#' && paramChannels[i][0] != '&')
		{
			Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), paramChannels[i]), client->getFd());
			continue ;
		}
		for (size_t j = 0; j < channels.size(); j++)
		{
			if (channels[j]->getName() == paramChannels[i])
			{
				channelExists = true;
				if (channels[j]->isMember(client))
				{
					Server::sendReply(ERR_USERONCHANNEL(client->getNickname(), paramChannels[i]), client->getFd());
					break ;
				}
				if (i < paramKeys.size() && channels[j]->getKey() != paramKeys[i])
				{
					Server::sendReply(ERR_BADCHANNELKEY(client->getNickname(), paramChannels[i]), client->getFd());
					break ;
				}
				channels[j]->addMember(client);
				break ;
			}
		}
		if (!channelExists)
		{
			Channel *newChannel = new Channel(paramChannels[i]);
			newChannel->addMember(client);
			newChannel->giveOperator(client);
			server.addChannel(newChannel);
		}
	}
}