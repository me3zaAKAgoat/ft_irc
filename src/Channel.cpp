#include "Irc.hpp"

Channel::Channel(const std::string name) : name(name), topic(""), key(""), isInviteOnly(false), channelTopicIsRestricted(false), limit(-1)
{
}

Channel::~Channel()
{
}

void Channel::addMember(Client *client)
{
	ChannelMember *member = new ChannelMember;

	member->client = client;
	member->isOperator = false;
	this->members.insert(std::make_pair(client->getFd(), member));
}

void Channel::removeMember(Client *client)
{
	delete this->members[client->getFd()];
	this->members.erase(client->getFd());

	// for (size_t i = 0; i < this->members.size(); i++)
	// {
	// 	if (this->members[i]->client == client)
	// 	{
	// 		delete this->members[i];
	// 		this->members.erase(this->members.begin() + i);
	// 		return ;
	// 	}
	// }
}

void Channel::giveOperator(Client *client)
{
	this->members[client->getFd()]->isOperator = true;
	// for (size_t i = 0; i < this->members.size(); i++)
	// {
	// 	if (this->members[i]->client == client)
	// 	{
	// 		this->members[i]->isOperator = true;
	// 		return ;
	// 	}
	// }
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

void Channel::setLimit(const int16_t limit)
{
	this->limit = limit;
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

int16_t Channel::getLimit(void)
{
	return (this->limit);
}

bool Channel::getInviteOnly(void)
{
	return (this->isInviteOnly);
}

bool Channel::getChannelTopicIsRestricted(void)
{
	return (this->channelTopicIsRestricted);
}

std::map<unsigned int, ChannelMember *> Channel::getMembers(void)
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

void Channel::broadcastMessage(Client *sender, const std::string message)
{
	std::map<unsigned int, ChannelMember *>::iterator it;
	for (it = this->members.begin(); it != this->members.begin(); it++)
	{
		if (it->second->client == sender)
			continue ;
		Server::sendReply(message, it->second->client->getFd());
	}
}

bool	Channel::isMember(Client *client)
{
	if (this->members.find(client->getFd()) != this->members.end())
		return (true);
	return (false);
	// for (size_t i = 0; i < this->members.size(); i++)
	// {
	// 	if (this->members[i]->client == client)
	// 		return true;
	// }
	// return false;
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
