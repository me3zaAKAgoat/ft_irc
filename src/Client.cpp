#include "Client.hpp"

Client::Client()
{
	// all this should be deleted it makes no sense
	this->fd = -1;
	this->isAuthenticated = false;
	this->nickNameIsSet = false;
	this->password = "passwordEmpty";
	this->nickName = "nicknameEmpty";
	this->loginName = "loginnameEmpty";
	this->realName = "realnameEmpty";
}

Client::Client(unsigned int fd) : fd(fd)
{
	this->isAuthenticated = false;
	this->nickNameIsSet = false;
}

// setters:
void Client::setFd(const unsigned int fd)
{
	this->fd = fd;
}

void	Client::authenticate(void)
{
	this->isAuthenticated = true;
}
void Client::setPassword(const std::string password)
{
	this->password = password;
}

void Client::setNickName(const std::string nickName)
{
	this->nickName = nickName;
	this->nickNameIsSet = true;
}

void Client::setLoginName(const std::string loginName)
{
	this->loginName = loginName;
}

void Client::setRealName(const std::string realName)
{
	this->realName = realName;
}

void	Client::setUserName(const std::string LoginName, const std::string RealName)
{
	this->setLoginName(LoginName);
	this->setRealName(RealName);
	this->userNameIsSet = true;
}

// getters:
unsigned int Client::getFd(void)
{
	return (this->fd);
}

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

bool	Client::isUserNameSet(void)
{
	return (this->userNameIsSet);
}

bool	Client::isAuthenticate(void)
{
	return (this->isAuthenticated);
}

bool	Client::isNickNameSet(void)
{
	return (this->nickNameIsSet);
}
