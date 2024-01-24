#pragma once

#include "Irc.hpp"

class Client
{
	public:
		Client();
		Client(unsigned int fd);
		// Client(const Client& copy);
		// Client& operator=(const Client& rhs);
		// ~Client();

		// setters:
		void	authenticate(void);
		void	setFd(const unsigned int fd);
		void	setPassword(const std::string Password);
		void	setNickname(const std::string Nickname);
		void	setUserName(const std::string Username, const std::string Realname);

		// getters:
		unsigned int	getFd(void);
		bool			isAuthenticate(void);
		bool			isNicknameSet(void);
		std::string		getPassword(void);
		std::string		getNickname(void);
		bool			isUserNameSet(void);
		std::string		getUsername(void);
		std::string		getRealname(void);

	private:

		void			setUsername(const std::string username);
		void			setRealname(const std::string realname);

		int	fd;

		bool			isAuthenticated;
		bool			nicknameIsSet;
		bool			usernameIsSet;
		std::string		nickname;
		std::string		password;
		std::string		username;
		std::string		realname;
};
