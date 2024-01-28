#include "Irc.hpp"

bool isExistChannel(Server &server, size_t &channelIndex, const std::string &channelName)
{
	std::vector<Channel *> channels = server.getChannels();
	for (channelIndex = 0; channelIndex < channels.size(); channelIndex++)
	{
		if (channels[channelIndex]->getName() == channelName)
			return (true);
	}
	return (false);
}

bool isMemberOperator(Channel *channel, Client *client)
{
	if (!channel->isMember(client))
	{
		Server::sendReply(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()), client->getFd());
		return (false);
	}
	if (!channel->getMembers()[client->getFd()]->isOperator)
	{
		Server::sendReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()), client->getFd());
		return (false);
	}
	return (true);
}

bool strIsDigits(const std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			return (false);
	}
	return (true);
}