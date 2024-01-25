#include "Client.hpp"

Client::~Client()
{
	close(this->fd);
}

Client::Client(unsigned int fd) : fd(fd), _isAuthenticated(false) // emptiness of strings may be allowed in irc not sure !
{
}

// setters:
void Client::setFd(const unsigned int fd)
{
	this->fd = fd;
}

void	Client::authenticate(void)
{
	this->_isAuthenticated = true;
}
void Client::setPassword(const std::string password)
{
	this->password = password;
}

void Client::setNickname(const std::string nickname)
{
	this->nickname = nickname;
}

void Client::setUsername(const std::string username)
{
	this->username = username;
}

void Client::setRealname(const std::string realname)
{
	this->realname = realname;
}

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

bool	Client::isAuthenticated(void)
{
	return (this->_isAuthenticated);
}