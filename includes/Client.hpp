#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"

class Client
{
public:
	Client();
	// Client(const Client& copy);
	// Client& operator=(const Client& rhs);
	// ~Client();

	// setters:
	void	setPassword(const std::string Password);
	void	setNickName(const std::string NickName);
	void	setLoginName(const std::string LoginName);
	void	setRealName(const std::string RealName);

	// getters:
	std::string	getPassword(void);
	std::string	getNickName(void);
	std::string	getLoginName(void);
	std::string	getRealName(void);

private:
	std::string nickName;
	std::string password;
	std::string loginName;
	std::string realName;
};

#endif