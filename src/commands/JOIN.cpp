#include "Irc.hpp"


/* 
there's more stuff that hasnt been implemented yet 
https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
*/
void	joinCmd(commandData& cmd, Server& server, Client* client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(": 461 " + client->getNickname() + " JOIN :Not enough parameters\r\n", client->getFd());
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
			Server::sendReply(": 403 " + client->getNickname() + " " + paramChannels[i] + " :No such channel\r\n", client->getFd());
			continue ;
		}
		for (size_t j = 0; j < channels.size(); j++)
		{
			if (channels[j]->getName() == paramChannels[i])
			{
				channelExists = true;
				if (channels[j]->isMember(client))
				{
					Server::sendReply(": 443 " + client->getNickname() + " " + paramChannels[i] + " :is already on channel\r\n", client->getFd());
					break ;
				}
				if (i <= paramKeys.size() && channels[j]->getKey() != paramKeys[i])
				{
					Server::sendReply(": 475 " + client->getNickname() + " " + paramChannels[i] + " :Cannot join channel (+k)\r\n", client->getFd());
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