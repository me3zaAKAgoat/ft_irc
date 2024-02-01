#include "Irc.hpp"

/*
i - invite-only channel flag; 					 	  || MODE #channel +i
t - topic settable by channel operator only flag;	  || MODE #channel +t
k - set a channel key (password).				 	  || MODE #channel +k "new-key"
o - give/take channel operator privileges;			  || MODE #channel +o nick
l - set the user memberLimit to channel;					  || MODE #channel +l 50
*/

void handleInviteFlag(Channel *channel, bool plusSign)
{
	if (plusSign)
		channel->setInviteOnly(true);
	else
		channel->setInviteOnly(false);
}

void handleTopicFlag(Channel *channel, bool plusSign)
{
	if (plusSign)
		channel->setChannelTopicIsRestricted(true);
	else
		channel->setChannelTopicIsRestricted(false);
}

void handleKeyFlag(Channel *channel, bool plusSign, std::string& key)
{
	if (plusSign)
		channel->setKey(key);
	else
		channel->setKey("");
}

void handleOperatorFlag(Channel *channel, bool plusSign, Client& target)
{
	if (plusSign)
		channel->giveOperator(&target);
	else
		channel->removeOperator(&target);
}

void handleLimitFlag(Channel *channel, bool plusSign, std::string& memberLimit)
{
	if (plusSign)
		channel->setmemberLimit(std::atoi(memberLimit.c_str()));
	else
		channel->setmemberLimit(-1);
}

bool is_digits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

bool	hasRequiredArgument(std::vector<std::string>::iterator flagArgIt, commandData& cmd, Client& client)
{
	if (flagArgIt == cmd.arguments.end())
		return (Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd()), false);
	return (true);
}

bool	isValidLimitArg(const std::string limitArg, Client &client, char flagMode)
{
	if (limitArg[0] != '+' && !is_digits(limitArg))
		return (Server::sendReply(ERR_INVALIDMODEPARAM(client.getNickname(), flagMode, limitArg), client.getFd()), false);
	if (std::atoi((limitArg).c_str()) <= 0)
		return (Server::sendReply(ERR_INVALIDMODEPARAM(client.getNickname(), flagMode, limitArg), client.getFd()) ,false);
	return (true);
}

bool	validateModeCommand(commandData& cmd, Client& client, Server& server)
{
	if (cmd.arguments.size() < 2)
		return (Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd()), false);
	if (!Channel::isValidChannelName(cmd.arguments[0]))
		return (Server::sendReply(ERR_NOSUCHCHANNEL(client.getNickname(), cmd.arguments[0]), client.getFd()), false);
	Channel *channel = server.getChannelByName(cmd.arguments[0]);
	if (!channel)
		return (Server::sendReply(ERR_NOSUCHCHANNEL(client.getNickname(), cmd.arguments[0]), client.getFd()), false);
	if (!channel->isMember(&client))
		return (Server::sendReply(ERR_NOTONCHANNEL(client.getNickname(), cmd.arguments[0]), client.getFd()), false);
	if (!channel->isOperator(&client))
		return (Server::sendReply(ERR_CHANOPRIVSNEEDED(client.getNickname(), cmd.arguments[0]), client.getFd()), false);
	return (true);
}

bool	isCurrentKey(std::string& keyToCheck, std::string currentChannelKey, Client &client)
{
	if (keyToCheck != currentChannelKey)
		return (Server::sendReply(ERR_INVALIDMODEPARAM(client.getNickname(), "k", keyToCheck), client.getFd()), false);
	return (true);
}

void		modeCmd(commandData& cmd, Server& server, Client& client)
{
	if (!validateModeCommand(cmd, client, server))
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
		return ;
	}
	bool								plusSign = true;
	std::string							firstArg = cmd.arguments[1];
	std::vector<std::string>::iterator	flagArgIt = cmd.arguments.begin() + 2;
	Channel								*channel = server.getChannelByName(cmd.arguments[0]);
	for (size_t i = 0; i < firstArg.size(); i++)
	{
		if (firstArg[i] == '+')
			plusSign = true;
		else if (firstArg[i] == '-')
			plusSign = false;
		else
		{
			if (firstArg[i] == 'i')
			{
				handleInviteFlag(channel, plusSign);
				Server::sendReply(RPL_MODE(client.getNickname(), cmd.arguments[0], firstArg[i], std::string("")), client.getFd());
			}
			else if (firstArg[i] == 't')
			{
				handleTopicFlag(channel, plusSign);
				Server::sendReply(RPL_MODE(client.getNickname(), cmd.arguments[0], firstArg[i], std::string("")), client.getFd());
			}
			else if (firstArg[i] == 'k')
			{
				if (!hasRequiredArgument(flagArgIt, cmd, client) || (!plusSign && !isCurrentKey(*flagArgIt, channel->getKey(), client)))
				{
					flagArgIt++; // {MODE #chan +lk a keypass} : the k flag take (keypass) not (a) as argument
					continue;
				}
				handleKeyFlag(channel, plusSign, *flagArgIt);
				Server::sendReply(RPL_MODE(client.getNickname(), cmd.arguments[0], (plusSign ? (std::string("+") + firstArg[i]) : (std::string("-") + firstArg[i])), (*flagArgIt)), client.getFd());
				flagArgIt++;
			}
			else if (firstArg[i] == 'o')
			{
				if (!hasRequiredArgument(flagArgIt, cmd, client))
				{
					flagArgIt++;
					continue;
				}
				Client *target = server.getClientByNickname(*flagArgIt);
				if (!target)
				{
					Server::sendReply(ERR_NOSUCHNICK(client.getNickname(), *flagArgIt), client.getFd());
					flagArgIt++;
					continue ;
				}
				handleOperatorFlag(channel, plusSign, *target);
				Server::sendReply(RPL_MODE(client.getNickname(), cmd.arguments[0], (plusSign ? (std::string("+") + firstArg[i]) : (std::string("-") + firstArg[i])), (plusSign ? *flagArgIt : std::string(""))), client.getFd());
				flagArgIt++;
			}
			else if (firstArg[i] == 'l')
			{
				if (!hasRequiredArgument(flagArgIt, cmd, client) || !isValidLimitArg((*flagArgIt), client, firstArg[i]))
				{
					flagArgIt++;
					continue;
				}
				handleLimitFlag(channel, plusSign, *flagArgIt);
				Server::sendReply(RPL_MODE(client.getNickname(), cmd.arguments[0], (plusSign ? (std::string("+") + firstArg[i]) : (std::string("-") + firstArg[i])), (plusSign ? *flagArgIt : std::string(""))), client.getFd());
				flagArgIt++;
			}
			else
			{
				Server::sendReply(ERR_UNKNOWNMODE(client.getNickname(), firstArg[i]), client.getFd());
				continue;
			}
		}
	}
}
