#include "Irc.hpp"

void	topicCmd(commandData& cmd, Server& server, Client& client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(RPL_NOTOPIC(client.getNickname(), ""), client.getFd());
		return ;
	}
	if (cmd.arguments[0][0] != '#' && cmd.arguments[0][0] != '&')
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
	if (cmd.arguments.size() > 1)
	{
		if (!channel->isOperator(&client))
		{
			Server::sendReply(ERR_CHANOPRIVSNEEDED(client.getNickname(), cmd.arguments[0]), client.getFd());
			return ;
		}
		channel->setTopic(cmd.arguments[1]);
	}
	else
	{
		if (channel->getTopic().empty())
			Server::sendReply(RPL_NOTOPIC(client.getNickname(), cmd.arguments[0]), client.getFd());
		else
			Server::sendReply(RPL_TOPIC(client.getNickname(), cmd.arguments[0], channel->getTopic()), client.getFd());
	}	
}