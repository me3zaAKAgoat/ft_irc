#include "Irc.hpp"

void	inviteCmd(commandData &cmd, Server &server, Client *client)
{
	if (cmd.arguments.size() < 2)
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client->getNickname(), cmd.name), client->getFd());
		return ;
	}
	if (cmd.arguments[1][0] != '#' && cmd.arguments[1][0] != '&')
	{
		Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[0]), client->getFd());
		return ;
	}
	Channel *channel = server.getChannelByName(cmd.arguments[1]);
	if (!channel)
	{
		Server::sendReply(ERR_NOSUCHCHANNEL(client->getNickname(), cmd.arguments[1]), client->getFd());
		return ;
	}
	if (!channel->isMember(client))
	{
		Server::sendReply(ERR_NOTONCHANNEL(client->getNickname(), cmd.arguments[0]), client->getFd());
		return ;
	}
	if (channel->getInviteOnly() && !channel->isOperator(client))
	{
		Server::sendReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), cmd.arguments[1]), client->getFd());
		return ;
	}
	Client *target = server.getClientByNickname(cmd.arguments[1]);
	if (!target)
	{
		Server::sendReply(ERR_NOSUCHNICK(client->getNickname(), cmd.arguments[0]), client->getFd());
		return ;
	}
	if (channel->isMember(target))
	{
		Server::sendReply(ERR_USERONCHANNEL(client->getNickname(), cmd.arguments[1]), client->getFd());
		return ;
	}
	channel->addMember(target);
}