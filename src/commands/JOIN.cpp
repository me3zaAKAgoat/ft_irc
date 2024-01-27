#include "Irc.hpp"


/* 
there's more stuff that hasnt been implemented yet 
https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
*/
void	joinCmd(commandData& cmd, Server& server, Client* client)
{
	std::vector<Channel *> channels = server.getChannels();
	bool	channelExists; 

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
				if (channels[j]->isMember(client))
				{
					Server::sendReply(": 443 " + client->getNickname() + " " + cmd.arguments[i] + " :is already on channel\r\n", client->getFd());
					break ;
				}
				channels[j]->addMember(client);
				channels[j]->giveOperator(client);
				break ;
			}
		}
		if (!channelExists)
		{
			Channel *newChannel = new Channel(cmd.arguments[i]);
			newChannel->addMember(client);
			newChannel->giveOperator(client);
			server.addChannel(newChannel);
		}
	}
}