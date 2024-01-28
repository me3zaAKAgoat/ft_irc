#include "Commands.hpp"

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

void handleChannelKeyMode(Channel *channel, bool sign, std::string& argumentFlag)
{
	if (sign)
		channel->setKey(argumentFlag);
	else
		channel->setKey("");
}

void handleChannelLimitMode(Client &client, Channel *channel, bool sign, std::string& argumentFlag)
{
	if (sign)
	{
		if (strIsDigits(argumentFlag) && atoi(argumentFlag.c_str()) > 0) // need to inform a msg about error
			channel->setLimit(atoi(argumentFlag.c_str()));
		else
			Server::sendReply(ERR_UNKNOWNMODE(client.getNickname(), channel->getName(), argumentFlag), client.getFd());
	}
	else
		channel->setLimit(-1);
}

void handleChannelOperatorMode(Channel *channel, Server &server, bool sign, std::string& argumentFlag)
{
	Client *client = NULL;
	if (sign)
	{
		client = server.getClientByNickname(argumentFlag);
		if (client)
		{
			if (channel->isMember(client) && !channel->isOperator(client))
				channel->giveOperator(client);
		}
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
		if (!isMemberOperator(channels[j], client))
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
				else if (cmd.arguments.size() > argCounter)
				{
					if (cmd.arguments[1][i] == 'k')
						handleChannelKeyMode(channels[j], sign, cmd.arguments[argCounter++]);
					else if (cmd.arguments[1][i] == 'l')
						handleChannelLimitMode((*client), channels[j], sign, cmd.arguments[argCounter++]);
					else if (cmd.arguments[1][i] == 'o')
						handleChannelOperatorMode(channels[j], server, sign, cmd.arguments[argCounter++]);
				}
				else
					Server::sendReply(ERR_NEEDMOREPARAMS((client ? client->getNickname() : "*"), cmd.name), client->getFd());
			}
			else
				Server::sendReply(ERR_UNKNOWNMODE(client->getNickname(), cmd.arguments[0], cmd.arguments[1][i]), client->getFd());
		}
	}
	else
		Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[0]), client->getFd());
}
