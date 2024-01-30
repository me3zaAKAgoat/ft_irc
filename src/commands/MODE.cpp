#include "Irc.hpp"

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
	Server::sendReply(RPL_MODE(client.getNickname(), cmd.arguments[0], cmd.arguments[1], (cmd.arguments.size() > 2 ? cmd.arguments[1] : std::string(""))), client.getFd());
	/* handle modes */
}
