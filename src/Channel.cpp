#include "Irc.hpp"

Channel::Channel(const std::string name) : name(name), isInviteOnly(false), channelTopicIsRestricted(false), limit(-1)
{
}

Channel::~Channel()
{
}