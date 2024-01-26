#include "Irc.hpp"

void	joinCmd(commandData& cmd, Server& server, Client* client)
{
	std::vector<Channel *> channels = server.getChannels();
	bool	channelExists; 

	for (size_t i = 0; i < cmd.arguments.size(); i++)
	{
		channelExists = false; 
		if (cmd.arguments[i][0] != '#' && cmd.arguments[i][0] != '&')
		{
			Server::sendResponse(": 403 " + client->getNickname() + " " + cmd.arguments[i] + " :No such channel\r\n", client->getFd());
			continue ;
		}
		for (size_t j = 0; j < channels.size(); j++)
		{
			if (channels[j]->getName() == cmd.arguments[i])
			{
				channelExists = true;
				channels[j]->addMember(client);
				break ;
			}
		}
		if (!channelExists)
		{
			Channel *newChannel = new Channel(cmd.arguments[i]);
			newChannel->addMember(client);
			server.addChannel(newChannel);
		}
	}
}