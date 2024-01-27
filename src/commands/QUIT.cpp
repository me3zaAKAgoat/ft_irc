#include "Irc.hpp"

void	quitCmd(commandData& cmd, Server& server, Client* client)
{
	std::vector<Channel *> channels = server.getChannels();
	std::string				quitMessage = cmd.arguments.size() > 0 ? cmd.arguments[0] : "No reason given";

	for (size_t j = 0; j < channels.size(); j++)
	{
		if (channels[j]->isMember(client))
		{
			channels[j]->broadcastMessage(client, ":" + client->getNickname() + " QUIT " + quitMessage + "\r\n");
			channels[j]->removeMember(client);
		}
	}
	server.closeConnection(client->getFd());
}