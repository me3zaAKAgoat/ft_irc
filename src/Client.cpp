#include "Client.hpp"

Client::Client()
{
	this->password = "";
	this->nickName = "";
	this->loginName = "";
	this->realName = "";
}

// setters:
void Client::setPassword(const std::string password)
{
	this->password = password;
}

void Client::setNickName(const std::string nickName)
{
	this->nickName = nickName;
}

void Client::setLoginName(const std::string loginName)
{
	this->loginName = loginName;
}

void Client::setRealName(const std::string realName)
{
	this->realName = realName;
}

// getters:
std::string Client::getPassword(void)
{
	return (this->password);
}

std::string Client::getNickName(void)
{
	return (this->nickName);
}

std::string Client::getLoginName(void)
{
	return (this->loginName);
}

std::string Client::getRealName(void)
{
	return (this->realName);
}
