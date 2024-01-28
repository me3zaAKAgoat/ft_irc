#include "Commands.hpp"

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

bool isMemberOperator(Channel *channel, Client *client, const std::string &channelName)
{
	if (!channel->isMember(client))
	{
		Server::sendReply(ERR_NOTONCHANNEL(client->getNickname(), channelName), client->getFd());
		return (false);
	}
	if (!channel->getMembers()[client->getFd()]->isOperator)
	{
		Server::sendReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName), client->getFd());
		return (false);
	}
	return (true);
}

bool isFlagMode(char c)
{
	if (c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l')
		return (true);
	return (false);
}

void handleInviteOnlyMode(Channel *channel, bool sign)
{
	if (sign)
	{
		if (!channel->getInviteOnly())
			channel->setInviteOnly(true);
	}
	else
	{
		if (channel->getInviteOnly())
			channel->setInviteOnly(false);
	}
}

void handleChannelTopicMode(Channel *channel, bool sign)
{
	if (sign)
	{
		if (!channel->getChannelTopicIsRestricted())
			channel->setChannelTopicIsRestricted(true);
	}
	else
	{
		if (channel->getChannelTopicIsRestricted())
			channel->setChannelTopicIsRestricted(false);
	}
}

void handleChannelKeyMode(Channel *channel, bool sign, std::vector<std::string> cmdArguments, size_t &argCounter)
{
	if (sign)
	{
		if (cmdArguments.size() > argCounter)
			channel->setKey(cmdArguments[argCounter++]);
	}
	else
		channel->setKey("");
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

void handleChannelLimitMode(Channel *channel, bool sign, std::vector<std::string> cmdArguments, size_t &argCounter)
{
	if (sign)
	{
		if (cmdArguments.size() > argCounter && strIsDigits(cmdArguments[argCounter]) && atoi(cmdArguments[argCounter++].c_str()) > 0) // need to inform a msg about error
			channel->setLimit(atoi(cmdArguments[argCounter++].c_str()));
	}
	else
		channel->setLimit(-1);
}

bool whichSign(char c, bool &sign)
{
	if (c == '-')
	{
		sign = false;
		return (true);
	}
	else if (c == '+')
	{
		sign = true;
		return (true);
	}
	return (false);
}

void modeCmd(commandData &cmd, Server &server, Client *client)
{
	// +ik-l pass 10
	// i - invite-only channel flag; 						MODE #channel -i 	  || MODE #channel +i
	// t - topic settable by channel operator only flag;	MODE #channel -t	  || MODE #channel +t
	// k - set a channel key (password).					MODE #channel -k 	  || MODE #channel +k "new-key"
	// o - give/take channel operator privileges;			MODE #channel -o nick || MODE #channel +o nick
	// l - set the user limit to channel;					MODE #channel -l	  || MODE #channel +l 50

	std::vector<Channel *> channels = server.getChannels();
	size_t argCounter = 2;
	if (cmd.arguments.size() < 2)
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), cmd.name), client->getFd());
		return;
	}
	if (cmd.arguments[0][0] != '#' && cmd.arguments[0][0] != '&')
	{
		Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[0]), client->getFd());
		return;
	}
	size_t j = -1;
	if (isExistChannel(server, j, cmd.arguments[0]))
	{
		if (!isMemberOperator(channels[j], client, cmd.arguments[0]))
			return;
		bool sign;
		size_t i = 0;
		if (!whichSign(cmd.arguments[1][i], sign))
			Server::sendReply(ERR_UNKNOWNMODE(client->getNickname(), cmd.arguments[0], cmd.arguments[1][i]), client->getFd());
		i++;
		while (i < cmd.arguments[1].size())
		{
			if (whichSign(cmd.arguments[1][i], sign))
				i++;
			else if (isFlagMode(cmd.arguments[1][i]))
			{
				if (cmd.arguments[1][i] == 'i')
					handleInviteOnlyMode(channels[j], sign);
				else if (cmd.arguments[1][i] == 't')
					handleChannelTopicMode(channels[j], sign);
				if (cmd.arguments[1][i] == 'k')
					handleChannelKeyMode(channels[j], sign, cmd.arguments, argCounter);
				if (cmd.arguments[1][i] == 'l')
					handleChannelLimitMode(channels[j], sign, cmd.arguments, argCounter);
				if (cmd.arguments[1][i] == 'o' && sign)
				{
				}
				else if (cmd.arguments[1][i] == 'o')
				{
				}
			}
			else
				Server::sendReply(ERR_UNKNOWNMODE(client->getNickname(), cmd.arguments[0], cmd.arguments[1][i]), client->getFd());
		}
	}
	else
		Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[0]), client->getFd());
}
