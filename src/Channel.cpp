#include "Irc.hpp"

Channel::Channel(const std::string name) : name(name), topic(""), key(""), isInviteOnly(false), channelTopicIsRestricted(false), memberLimit(-1)
{
}

Channel::~Channel()
{
	for (size_t i = 0; i < this->members.size(); i++)
		delete this->members[i];
}

void Channel::addMember(Client *client)
{
	if (this->memberLimit != -1 && this->members.size() >= static_cast<size_t>(this->memberLimit))
	{
		Server::sendReply(ERR_CHANNELISFULL(client->getNickname(), this->getName()), client->getFd());
		return ;
	}
	ChannelMember *member = new ChannelMember;

	member->client = client;
	member->isOperator = false;
	this->members.push_back(member);
}

void Channel::removeMember(Server &server, Client *client)
{
	for (size_t i = 0; i < this->members.size(); i++)
	{
		if (this->members[i]->client == client)
		{
			delete this->members[i];
			this->members.erase(this->members.begin() + i);
			break ;
		}
	}
	if (!this->getMembers().size())
		server.removeChannel(this);
}

void Channel::giveOperator(Client *client)
{
	for (size_t i = 0; i < this->members.size(); i++)
	{
		if (this->members[i]->client == client)
		{
			this->members[i]->isOperator = true;
			return ;
		}
	}
}

void Channel::removeOperator(Client *client)
{
	for (size_t i = 0; i < this->members.size(); i++)
	{
		if (this->members[i]->client == client)
		{
			this->members[i]->isOperator = false;
			return ;
		}
	}
}

void Channel::setName(const std::string name)
{
	this->name = name;
}

void Channel::setKey(const std::string key)
{
	this->key = key;
}

void Channel::setmemberLimit(const int16_t memberLimit)
{
	this->memberLimit = memberLimit;
}

void Channel::setInviteOnly(const bool isInviteOnly)
{
	this->isInviteOnly = isInviteOnly;
}

void Channel::setChannelTopicIsRestricted(const bool channelTopicIsRestricted)
{
	this->channelTopicIsRestricted = channelTopicIsRestricted;
}

std::string Channel::getName(void)
{
	return (this->name);
}

std::string Channel::getKey(void)
{
	return (this->key);
}

int16_t Channel::getmemberLimit(void)
{
	return (this->memberLimit);
}

bool Channel::getInviteOnly(void)
{
	return (this->isInviteOnly);
}

bool Channel::getChannelTopicIsRestricted(void)
{
	return (this->channelTopicIsRestricted);
}

std::vector<ChannelMember *> Channel::getMembers(void)
{
	return (this->members);
}

std::string Channel::getTopic(void)
{
	return (this->topic);
}

void Channel::setTopic(const std::string topic)
{
	this->topic = topic;
}

/* NULL sender is a server sent message */
void Channel::broadcastMessage(Client *sender, const std::string message)
{
	for (size_t i = 0; i < this->members.size(); i++)
	{
		if (sender && this->members[i]->client == sender)
			continue ;
		Server::sendReply(message, this->members[i]->client->getFd());
	}
}

bool	Channel::isMember(Client *client)
{
	for (size_t i = 0; i < this->members.size(); i++)
	{
		if (this->members[i]->client == client)
			return true;
	}
	return false;
}

bool	Channel::isOperator(Client *client)
{
	for (size_t i = 0; i < this->members.size(); i++)
	{
		if (this->members[i]->client == client)
			return this->members[i]->isOperator;
	}
	return false;
}

bool	Channel::isValidChannelName(const std::string name)
{
	if (name[0] != '#' && name[0] != '&')
		return false;
	for (size_t i = 1; i < name.size(); i++)
	{
		if (name[i] == ',' || name[i] == ' ')
			return false;
	}
	return true;
}