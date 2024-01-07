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
	void	setPassword(const string Password);
	void	setNickName(const string NickName);
	void	setLoginName(const string LoginName);
	void	setRealName(const string RealName);

	// getters:
	string	getPassword(void);
	string	getNickName(void);
	string	getLoginName(void);
	string	getRealName(void);

private:
	string nickName;
	string password;
	string loginName;
	string realName;
};

#endif