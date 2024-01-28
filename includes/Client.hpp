#pragma once

#include "Irc.hpp"

class Server;

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

		unsigned int	getFd(void);
		std::string		getNickname(void);
		std::string		getUsername(void);
		std::string		getRealname(void);
		bool			isAuthenticated(void);
		bool			isRegistered(void);

	private:
		int				fd;

		std::string		nickname;
		std::string		username;
		std::string		realname;
		bool			_isAuthenticated;
		bool			_isRegistered;
};
