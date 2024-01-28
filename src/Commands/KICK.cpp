#include "Commands.hpp"

void	kickMembers(Server &server, const std::vector<std::string> &paramUsers, const std::string &channelName, Channel *channel)
{
	for (size_t k = 0; k < paramUsers.size(); k++)
	{
		Client *user = server.getClientByNickname(paramUsers[k]);
		if (user)
		{
			if (!channel->isMember(user))
				Server::sendReply(ERR_NOTONCHANNEL(user->getNickname(), channelName), user->getFd());
			else
			{
				channel->removeMember(user);
			}
		}
	}
}

bool	isExistChannel(Server &server, size_t &channelIndex, const std::string &channelName)
{
	std::vector<Channel *> channels = server.getChannels();
	for (channelIndex = 0; channelIndex < channels.size(); channelIndex++)
	{
		if (channels[channelIndex]->getName() == channelName)
			return (true);
	}
	return (false);
}

void	kickCmd(commandData& cmd, Server &server, Client* client)
{
	if (cmd.arguments.size() < 2)
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), cmd.name), client->getFd());
		return ;
	}

	std::vector<Channel *> channels = server.getChannels();
	std::vector<std::string> paramChannels = split(cmd.arguments[0], ",");
	std::vector<std::string> paramUsers = split(cmd.arguments[1], ",");

	for (size_t i = 0; i < paramChannels.size(); i++)
	{
		if (paramChannels[i][0] != '#' && paramChannels[i][0] != '&')
		{
			Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), paramChannels[i]), client->getFd());
			continue ;
		}
		size_t j = -1;
		if (isExistChannel(server, j, paramChannels[i]))
		{
			if (!channels[j]->isMember(client))
			{
				Server::sendReply(ERR_NOTONCHANNEL(client->getNickname(), paramChannels[i]), client->getFd());
				break;
			}
			if (!channels[j]->getMembers()[client->getFd()]->isOperator)
			{
				Server::sendReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), paramChannels[i]), client->getFd());
				break;
			}
			kickMembers(server, paramUsers, paramChannels[i], channels[j]);
		}
		else
			Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[i]), client->getFd());
	}
}
