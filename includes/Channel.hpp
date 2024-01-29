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

		void							addMember(Client *client);
		void							removeMember(Server &server, Client *client);
		void							giveOperator(Client *client);
		void							removeOperator(Client *client);
		void							broadcastMessage(Client *sender, const std::string message);
		bool							isMember(Client *client);
		bool							isOperator(Client *client);
		static bool						isValidChannelName(const std::string name);

		void							setName(const std::string name);
		void							setTopic(const std::string t);
		void							setKey(const std::string key);
		void							setLimit(const int16_t limit);
		void							setInviteOnly(const bool isInviteOnly);
		void							setChannelTopicIsRestricted(const bool channelTopicIsRestricted);

		std::string						getName(void);
		std::string						getTopic(void);
		std::string						getKey(void);
		int16_t							getLimit(void);
		bool							getInviteOnly(void);
		bool							getChannelTopicIsRestricted(void);
		std::vector<ChannelMember *>	getMembers(void);


	private:

		std::string						name;
		std::string						topic;
		std::string						key;
		bool							isInviteOnly;
		bool							channelTopicIsRestricted;
		int16_t							limit; // -1 if no limit
		std::vector<ChannelMember *>	members;
};
