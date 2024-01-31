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
	bool	channelExists; 


	if (cmd.arguments.size() > 1)
		paramKeys = split(cmd.arguments[1], ",");
	for (size_t i = 0; i < paramChannels.size(); i++)
	{
		channelExists = false; 
		if (!Channel::isValidChannelName(paramChannels[i]))
		{
			Server::sendReply(ERR_NOSUCHCHANNEL(client.getNickname(), paramChannels[i]), client.getFd());
			continue ;
		}
		for (size_t j = 0; j < channels.size(); j++)
		{
			if (channels[j]->getName() == paramChannels[i])
			{
				channelExists = true;
				if (channels[j]->isMember(&client))
				{
					Server::sendReply(ERR_USERONCHANNEL(client.getNickname(), paramChannels[i]), client.getFd());
					break ;
				}
				if (i < paramKeys.size() && channels[j]->getKey() != paramKeys[i])
				{
					Server::sendReply(ERR_BADCHANNELKEY(client.getNickname(), paramChannels[i]), client.getFd());
					break ;
				}
				if (channels[j]->getmemberLimit() != -1 && static_cast<int>(channels[j]->getMembers().size()) >= channels[j]->getmemberLimit())
				{
					Server::sendReply(ERR_CHANNELISFULL(client.getNickname(), paramChannels[i]), client.getFd());
					break ;
				}
				channels[j]->addMember(&client);

				if (channels[j]->getTopic() == "")
					Server::sendReply(RPL_NOTOPIC(client.getNickname(), channels[j]->getName()), client.getFd());
				else
					Server::sendReply(RPL_TOPIC(channels[j]->getName(), channels[j]->getTopic()) , client.getFd());
				std::vector<ChannelMember *> members = channels[j]->getMembers();
				std::vector<std::string> nicknames;
				for (size_t k = 0; k < members.size(); k++)
					nicknames.push_back("@" + members[k]->client->getNickname());
				Server::sendReply(RPL_NAMREPLY(channels[j]->getName(), join(nicknames)), client.getFd());

				break ;
			}
		}
		if (!channelExists)
		{
			Channel *newChannel = new Channel(paramChannels[i]);
			newChannel->addMember(&client);
			newChannel->giveOperator(&client);
			server.addChannel(newChannel);
		}
	}
}