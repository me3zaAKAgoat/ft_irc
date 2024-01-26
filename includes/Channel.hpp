#pragma once

#include "Irc.hpp"

typedef struct
{
	Client	*client;
	bool	isOperator;
}	ChannelMember;

class Channel
{
	public:
		Channel(const std::string name);
		~Channel();

	private:

		bool						isInviteOnly;
		bool						channelTopicIsRestricted;
		std::int16_t				limit; // -1 if no limit

		std::string					key;
		std::string					name;
		std::vector<ChannelMember>	members;
};
