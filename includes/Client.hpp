#pragma once

#include "Irc.hpp"

class Client
{
	public:
		Client();
		Client(unsigned int fd);
		// Client(const Client& copy);
		// Client& operator=(const Client& rhs);
		~Client();

		// setters:
		void	authenticate(void);
		void	setFd(const unsigned int fd);
		void	setPassword(const std::string Password);
		void	setNickname(const std::string Nickname);
		void	setUsername(const std::string username);
		void	setRealname(const std::string realname);

		// getters:
		unsigned int	getFd(void);
		bool			isAuthenticated(void);
		std::string		getPassword(void);
		std::string		getNickname(void);
		std::string		getUsername(void);
		std::string		getRealname(void);

	private:


		int				fd;

		std::string		nickname;
		std::string		password;
		std::string		username;
		std::string		realname;
		bool			_isAuthenticated;
};
