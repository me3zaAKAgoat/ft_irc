#include "Irc.hpp"


/* 
there's more stuff that hasnt been implemented yet

- send commands that the server receives which affect the channel
- RPL_NAMREPLY
https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1
*/
void	joinCmd(commandData& cmd, Server& server, Client& client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
		return ;
	}
	std::vector<Channel *> channels = server.getChannels();
	std::vector<std::string> paramChannels = split(cmd.arguments[0], ",");
	std::vector<std::string> paramKeys;


	if (cmd.arguments.size() > 1)
		paramKeys = split(cmd.arguments[1], ",");
	for (size_t i = 0; i < paramChannels.size(); i++)
	{
		if (!Channel::isValidChannelName(paramChannels[i]))
		{
			Server::sendReply(ERR_NOSUCHCHANNEL(client.getNickname(), paramChannels[i]), client.getFd());
			continue ;
		}
		Channel *channel = server.getChannelByName(paramChannels[i]);
		if (!channel)
		{
			Channel *newChannel = new Channel(paramChannels[i]);
			newChannel->addMember(&client);
			newChannel->giveOperator(&client);
			server.addChannel(newChannel);
			continue ;
		}
		if (channel->isMember(&client))
		{
			Server::sendReply(ERR_USERONCHANNEL(client.getNickname(), paramChannels[i]), client.getFd());
			continue ;
		}
		if (channel->getInviteOnly() && !client.isInvitedToChannel(paramChannels[i]))
		{
			Server::sendReply(ERR_INVITEONLYCHAN(paramChannels[i]), client.getFd());
			continue ;
		}
		if (!channel->getKey().empty())
		{
			if (i < paramKeys.size())
			{
				Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), paramChannels[i]), client.getFd());
				continue;
			}
			if (channel->getKey() != paramKeys[i])
			{
				Server::sendReply(ERR_BADCHANNELKEY(client.getNickname(), paramChannels[i]), client.getFd());
				continue ;
			}
		}
		if (channel->getmemberLimit() != -1 && static_cast<int>(channel->getMembers().size()) >= channel->getmemberLimit())
		{
			Server::sendReply(ERR_CHANNELISFULL(client.getNickname(), paramChannels[i]), client.getFd());
			continue;
		}
		channel->addMember(&client);
		if (channel->getInviteOnly())
			client.removeInviteToChannel(paramChannels[i]);
		if (channel->getTopic() == "")
			Server::sendReply(RPL_NOTOPIC(client.getNickname()), client.getFd());
		else
			Server::sendReply(RPL_TOPIC(channel->getName(), channel->getTopic()) , client.getFd());
		std::vector<ChannelMember *> members = channel->getMembers();
		std::vector<std::string> nicknames;
		for (size_t k = 0; k < members.size(); k++)
		nicknames.push_back("@" + members[k]->client->getNickname());
		Server::sendReply(RPL_NAMREPLY(channel->getName(), join(nicknames)), client.getFd());
	}
}