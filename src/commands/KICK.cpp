#include "Irc.hpp"

void	kickCmd(commandData& cmd, Server &server, Client &client)
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
	Client *target = server.getClientByNickname(cmd.arguments[1]);
	if (!target)
	{
		Server::sendReply(ERR_NOSUCHNICK(client.getNickname(), cmd.arguments[1]), client.getFd());
		return ;
	}
	if (!channel->isMember(target))
	{
		Server::sendReply(ERR_USERNOTINCHANNEL(client.getNickname(), cmd.arguments[1], cmd.arguments[0]), client.getFd());
		return ;
	}
	channel->removeMember(server, target);
	if (cmd.arguments.size() > 2)
	{
		channel->broadcastMessage(NULL, RPL_KICK(formulatePrefix(server.getHostname(), client.getNickname(), client.getUsername()), cmd.arguments[0], cmd.arguments[1], cmd.arguments[2]));
		Server::sendReply(RPL_KICK(":" + client.getNickname(), cmd.arguments[0], cmd.arguments[1], cmd.arguments[2]), target->getFd());
	}
	else
	{
		channel->broadcastMessage(NULL, RPL_KICK(formulatePrefix(server.getHostname(), client.getNickname(), client.getUsername()), cmd.arguments[0], cmd.arguments[1], ""));
		Server::sendReply(RPL_KICK(":" + client.getNickname(), cmd.arguments[0], cmd.arguments[1], ""), target->getFd());
	}
}