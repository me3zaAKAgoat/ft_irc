#include "Client.hpp"

Client::Client()
{
	// all this should be deleted it makes no sense
	this->fd = -1;
	this->isAuthenticated = false;
	this->nicknameIsSet = false;
	this->password = "passwordEmpty";
	this->nickname = "nicknameEmpty";
	this->username = "loginnameEmpty";
	this->realname = "realnameEmpty";
}

Client::~Client()
{
	close(this->fd);
}

Client::Client(unsigned int fd) : fd(fd)
{
	this->isAuthenticated = false;
	this->nicknameIsSet = false;
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

void Client::setNickname(const std::string nickname)
{
	this->nickname = nickname;
	this->nicknameIsSet = true;
}

void Client::setUsername(const std::string username)
{
	this->username = username;
}

void Client::setRealname(const std::string realname)
{
	this->realname = realname;
}

void	Client::setUserName(const std::string Username, const std::string Realname)
{
	this->setUsername(Username);
	this->setRealname(Realname);
	this->usernameIsSet = true;
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

std::string Client::getNickname(void)
{
	return (this->nickname);
}

std::string Client::getUsername(void)
{
	return (this->username);
}

std::string Client::getRealname(void)
{
	return (this->realname);
}

bool	Client::isUserNameSet(void)
{
	return (this->usernameIsSet);
}

bool	Client::isAuthenticate(void)
{
	return (this->isAuthenticated);
}

bool	Client::isNicknameSet(void)
{
	return (this->nicknameIsSet);
}
