#include "Irc.hpp"

void	topicCmd(commandData& cmd, Server& server, Client& client)
{
	if (!cmd.arguments.size())
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
	if (channel->getChannelTopicIsRestricted() && !channel->isOperator(&client))
	{
		Server::sendReply(ERR_CHANOPRIVSNEEDED(client.getNickname(), cmd.arguments[0]), client.getFd());
		return ;
	}
	if (cmd.arguments.size() > 1)
		channel->setTopic(cmd.arguments[1]);
	else
		channel->setTopic("");
	if (channel->getTopic().empty())
		channel->broadcastMessage(&client, RPL_NOTOPIC(client.getNickname(), cmd.arguments[0]));
	else
		channel->broadcastMessage(&client, RPL_TOPIC(client.getNickname(), cmd.arguments[0], channel->getTopic()));
}