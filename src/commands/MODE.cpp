#include "Irc.hpp"

/*
i - invite-only channel flag; 					 	  || MODE #channel +i
t - topic settable by channel operator only flag;	  || MODE #channel +t
k - set a channel key (password).				 	  || MODE #channel +k "new-key"
o - give/take channel operator privileges;			  || MODE #channel +o nick
l - set the user memberLimit to channel;			  || MODE #channel +l 50
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
		channel->setmemberLimit(0);
}

bool	isValidNum(const std::string &str)
{
	if (str[0] == '-' || str[0] == '+')
		return str.find_first_not_of("0123456789", 1) == std::string::npos;
	else
		return str.find_first_not_of("0123456789") == std::string::npos;
}

void		modeCmd(commandData& cmd, Server& server, Client& client)
{
	if (cmd.arguments.size() < 2)
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
		return ;
	}
	if (!Channel::isValidChannelName(cmd.arguments[0]))
	{
		Server::sendReply(ERR_NOSUCHCHANNEL(client.getNickname(), cmd.arguments[0]), client.getFd());
		return ;
	}
	Channel *channel = server.getChannelByName(cmd.arguments[0]);
	if (!channel)
	{
		Server::sendReply(ERR_NOSUCHCHANNEL(client.getNickname(), cmd.arguments[0]), client.getFd());
		return ;
	}
	if (!channel->isMember(&client))
	{
		Server::sendReply(ERR_NOTONCHANNEL(client.getNickname(), cmd.arguments[0]), client.getFd());
		return ;
	}
	if (!channel->isOperator(&client))
	{
		Server::sendReply(ERR_CHANOPRIVSNEEDED(client.getNickname(), cmd.arguments[0]), client.getFd());
		return ;
	}
	/*
	while loop over the first argument and then store whatever sign was encountered 
	and flavour every upcoming mode flag function with it (not sure if this is correct behaviour)
	*/
	bool								plusSign = true;
	std::string							firstArg = cmd.arguments[1];
	std::vector<std::string>::iterator	flagArgIt = cmd.arguments.begin() + 2;
	bool								foundAMode = false;
	for (size_t i = 0; i < firstArg.size(); i++)
	{
		if (firstArg[i] == '+')
			plusSign = true;
		else if (firstArg[i] == '-')
			plusSign = false;
		else
		{
			foundAMode = true;
			if (std::string("itkol").find(firstArg[i]) != std::string::npos)
			{
				if (firstArg[i] == 'i')
					handleInviteFlag(channel, plusSign);
				else if (firstArg[i] == 't')
					handleTopicFlag(channel, plusSign);
				else if (firstArg[i] == 'k')
				{
					if (flagArgIt == cmd.arguments.end())
					{
						Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
						continue;
					}
					if (!plusSign && *flagArgIt != channel->getKey())
					{
						Server::sendReply(ERR_INVALIDMODEPARAM(client.getNickname(), firstArg[i], *flagArgIt), client.getFd());
						continue;
					}
					handleKeyFlag(channel, plusSign, *flagArgIt);
					flagArgIt++;
				}
				else if (firstArg[i] == 'o')
				{
					if (flagArgIt == cmd.arguments.end())
					{
						Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
						continue;
					}
					Client *target = server.getClientByNickname(*flagArgIt);
					if (!target)
					{
						Server::sendReply(ERR_NOSUCHNICK(client.getNickname(), *flagArgIt), client.getFd());
						continue ;
					}
					handleOperatorFlag(channel, plusSign, *target);
					flagArgIt++;
				}
				else if (firstArg[i] == 'l')
				{
					if (flagArgIt == cmd.arguments.end())
					{
						Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
						continue;
					}
					if (isValidNum(*flagArgIt) == false)
					{
						Server::sendReply(ERR_INVALIDMODEPARAM(client.getNickname(), firstArg[i], *flagArgIt), client.getFd());
						continue;
					}
					if (std::atoi((*flagArgIt).c_str()) < 0)
					{
						Server::sendReply(ERR_INVALIDMODEPARAM(client.getNickname(), firstArg[i], *flagArgIt), client.getFd());
						continue;
					}
					handleLimitFlag(channel, plusSign, *flagArgIt);
					flagArgIt++;
				}
				if (firstArg[i] == 'i' || firstArg[i] == 't')
					channel->broadcastMessage(NULL, RPL_MODE(formulatePrefix(server.getHostname(), client.getNickname(), client.getUsername()), (cmd.arguments[0] + " " + (plusSign ? "+" : "-") + firstArg[i])));
				else if (firstArg[i] == 'k' || firstArg[i] == 'o' || firstArg[i] == 'l')
				{
					--flagArgIt;
					channel->broadcastMessage(NULL, RPL_MODE(formulatePrefix(server.getHostname(), client.getNickname(), client.getUsername()), (cmd.arguments[0] + " " + (plusSign ? "+" : "-") + firstArg[i] + " " + *flagArgIt)));
					++flagArgIt;
				}
			}
			else
			{
				Server::sendReply(ERR_UNKNOWNMODE(client.getNickname(), firstArg[i]), client.getFd());
				continue;
			}
		}
	}
	if (!foundAMode)
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
		return ;
	}
	// channel->broadcastMessage(NULL, RPL_MODE(formulatePrefix(server.getHostname(), client.getNickname(), client.getUsername()), join(cmd.arguments.begin(), cmd.arguments.end())));
}
