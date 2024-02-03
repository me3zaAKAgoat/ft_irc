#pragma once

#include "Irc.hpp"

class Client
{
	public:
		Client(unsigned int fd);
		~Client();

		void			authenticate(void);
		void			_register(void);
		void			setFd(const unsigned int fd);
		void			setNickname(const std::string Nickname);
		void			setUsername(const std::string username);
		void			setRealname(const std::string realname);
		void			concatCmdBuffer(std::string &buffer);
		void			clearCmdBuffer(void);

		std::string		getCmdBuffer(void);
		unsigned int	getFd(void);
		std::string		getNickname(void);
		std::string		getUsername(void);
		std::string		getRealname(void);
		bool			isAuthenticated(void);
		bool			isRegistered(void);
		void			addChannelInvitation(const std::string &channel);
		bool			isInvitedToChannel(const std::string &channel);
		void			removeInviteToChannel(const std::string& channel);

	private:
		int				fd;

		std::string		nickname;
		std::string		username;
		std::string		realname;
		bool			_isAuthenticated;
		bool			_isRegistered;
		std::vector<std::string>	channelInvitations;
		std::string		cmdBuffer;
};
