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
		void	setNickName(const std::string NickName);
		void	setUserName(const std::string LoginName, const std::string RealName);

		// getters:
		unsigned int	getFd(void);
		bool			isAuthenticate(void);
		bool			isNickNameSet(void);
		std::string		getPassword(void);
		std::string		getNickName(void);
		bool			isUserNameSet(void);
		std::string		getLoginName(void);
		std::string		getRealName(void);

	private:

		void			setLoginName(const std::string LoginName);
		void			setRealName(const std::string RealName);

		unsigned int	fd;
		bool			isAuthenticated;
		bool			nickNameIsSet;
		bool			userNameIsSet;
		std::string		nickName;
		std::string		password;
		std::string		loginName;
		std::string		realName;
};
