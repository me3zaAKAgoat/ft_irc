#include "Client.hpp"

Client::~Client()
{
	close(this->fd);
}

Client::Client(unsigned int fd) : fd(fd), _isAuthenticated(false), _isRegistered(false)
{
}

void Client::setFd(const unsigned int fd)
{
	this->fd = fd;
}

void Client::authenticate(void)
{
	this->_isAuthenticated = true;
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

bool Client::isAuthenticated(void)
{
	return (this->_isAuthenticated);
}

bool Client::isRegistered(void)
{
	return (this->_isRegistered);
}

void Client::_register(void)
{
	this->_isRegistered = true;
}

void Client::addInvitedChannel(const std::string channelName)
{
	this->invitedChannelsNames.push_back(channelName);
}

std::vector<std::string> Client::getInvitedChannelsNames(void)
{
	return (this->invitedChannelsNames);
}

bool Client::hasInvitationForChannel(std::string channelName)
{
	for (size_t i = 0; i < this->invitedChannelsNames.size(); i++)
		if (invitedChannelsNames[i] == channelName)
			return (true);
	return (false);
}
