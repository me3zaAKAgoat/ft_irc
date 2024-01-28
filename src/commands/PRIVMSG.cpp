#include "Irc.hpp"

void privMsgCmd(commandData& cmd, Server& server, Client& client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(ERR_NORECIPIENT(client.getNickname(), cmd.name), client.getFd());
		return ;
	}
	if (cmd.arguments.size() <= 1 || !cmd.arguments[1].size())
	{
		Server::sendReply(ERR_NOTEXTTOSEND(client.getNickname()), client.getFd());
		return ;
	}
	if (cmd.arguments[0][0] == '#' || cmd.arguments[0][0] == '&')
	{
		Channel *channel = server.getChannelByName(cmd.arguments[0]);
		if (!channel)
		{
			Server::sendReply(ERR_NOSUCHNICK(client.getNickname(), cmd.arguments[0]), client.getFd());
			return ;
		}
		// if (!channel->getMembers().size()) // a channel with no members is not possible
		// {
		// 	Server::sendReply(": 401 " + client.getNickname() + " " + cmd.arguments[0] + " :No such nick/channel\r\n", client.getFd());
		// 	return ;
		// }
		if (!channel->isMember(&client))
		{
			Server::sendReply(ERR_CANNOTSENDTOCHAN(client.getNickname(),cmd.arguments[0]), client.getFd());
			return ;
		}
		channel->broadcastMessage(&client, ":" + client.getNickname() + " PRIVMSG " + cmd.arguments[0] + " :" + cmd.arguments[1] + "\r\n");
	}
	else
	{
		Client *receiver = server.getClientByNickname(cmd.arguments[0]);
		if (!receiver)
		{
			Server::sendReply(ERR_NOSUCHNICK(client.getNickname(), cmd.arguments[0]), client.getFd());
			return ;
		}
		Server::sendReply(":" + client.getNickname() + " PRIVMSG " + cmd.arguments[0] + " :" + cmd.arguments[1] + "\r\n", receiver->getFd());
	}
}