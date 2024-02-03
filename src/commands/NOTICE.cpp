#include "Irc.hpp"

void noticeCmd(commandData& cmd, Server& server, Client& client)
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
	std::vector<std::string> receivers = split(cmd.arguments[0], ",");
	for (size_t i = 0; i < receivers.size(); i++)
	{
		if (receivers[i][0] == '#' || receivers[i][0] == '&')
		{
			Channel *channel = server.getChannelByName(receivers[i]);
			if (!channel)
			{
				Server::sendReply(ERR_NOSUCHCHANNEL(client.getNickname(), receivers[i]), client.getFd());
				return ;
			}
			if (!channel->isMember(&client))
			{
				Server::sendReply(ERR_NOTONCHANNEL(client.getNickname(),receivers[i]), client.getFd());
				return ;
			}
			channel->broadcastMessage(&client, RPL_NOTICE(client.getNickname(), receivers[i], cmd.arguments[1]));
		}
		else
		{
			Client *receiver = server.getClientByNickname(receivers[i]);
			if (!receiver)
			{
				Server::sendReply(ERR_NOSUCHNICK(client.getNickname(), receivers[i]), client.getFd());
				return ;
			}
			Server::sendReply(RPL_NOTICE(client.getNickname(), receivers[i], cmd.arguments[1]), receiver->getFd());
		}
	}
}