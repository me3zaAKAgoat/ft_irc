#include "Irc.hpp"

void	partCmd(commandData& cmd, Server& server, Client& client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
		return ;
	}
	std::vector<std::string> channelsToLeave = split(cmd.arguments[0], ",");
	for (size_t i = 0; i < channelsToLeave.size(); i++)
	{
		if (!Channel::isValidChannelName(channelsToLeave[i]))
		{
			Server::sendReply(ERR_NOSUCHCHANNEL(client.getNickname(), channelsToLeave[i]), client.getFd());
			continue ;
		}
		Channel *channel = server.getChannelByName(channelsToLeave[i]);
		if (!channel)
		{
			Server::sendReply(ERR_NOSUCHCHANNEL(client.getNickname(), channelsToLeave[i]), client.getFd());
			continue ;
		}
		if (!channel->isMember(&client))
		{
			Server::sendReply(ERR_NOTONCHANNEL(client.getNickname(), channelsToLeave[i]), client.getFd());
			continue ;
		}
		channel->removeMember(server, &client);
		Server::sendReply(RPL_PART(formulatePrefix(server.getHostname(), client.getNickname(), client.getUsername()), channel->getName(), (cmd.arguments.size() > 1 ? cmd.arguments[1] : "")), client.getFd());
		channel->broadcastMessage(&client, RPL_PART(formulatePrefix(server.getHostname(), client.getNickname(), client.getUsername()), channel->getName(), (cmd.arguments.size() > 1 ? cmd.arguments[1] : "")));
	}
}