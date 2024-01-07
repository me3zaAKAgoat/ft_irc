#include "Client.hpp"

Client::Client()
{
	this->password = "";
	this->nickName = "";
	this->loginName = "";
	this->realName = "";
}

// setters:
void Client::setPassword(const string password)
{
	this->password = password;
}

void Client::setNickName(const string nickName)
{
	this->nickName = nickName;
}

void Client::setLoginName(const string loginName)
{
	this->loginName = loginName;
}

void Client::setRealName(const string realName)
{
	this->realName = realName;
}

// getters:
string Client::getPassword(void)
{
	return (this->password);
}

string Client::getNickName(void)
{
	return (this->nickName);
}

string Client::getLoginName(void)
{
	return (this->loginName);
}

string Client::getRealName(void)
{
	return (this->realName);
}
