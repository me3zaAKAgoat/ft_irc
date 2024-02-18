#include "Client.hpp"

Client::~Client()
{
	close(this->fd);
}

Client::Client(unsigned int fd, std::string clientHostname) : fd(fd), hostname(clientHostname), _isAuthenticated(false), _isRegistered(false)
{
}

void Client::setFd(const unsigned int fd)
{
	this->fd = fd;
}

void	Client::authenticate(void)
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

std::string		Client::getHostname(void)
{
	return (this->hostname);
}

bool	Client::isAuthenticated(void)
{
	return (this->_isAuthenticated);
}

bool	Client::isRegistered(void)
{
	return (this->_isRegistered);
}

void	Client::_register(void)
{
	this->_isRegistered = true;
}

void	Client::addChannelInvitation(const std::string& channel)
{
	this->channelInvitations.push_back(channel);
}

bool	Client::isInvitedToChannel(const std::string& channel)
{
	for (size_t i = 0; i < this->channelInvitations.size(); i++)
	{
		if (this->channelInvitations[i] == channel)
			return (true);
	}
	return (false);
}

void	Client::removeInviteToChannel(const std::string& channel)
{
	for (size_t i = 0; i < this->channelInvitations.size(); i++)
	{
		if (this->channelInvitations[i] == channel)
		{
			this->channelInvitations.erase(this->channelInvitations.begin() + i);
			return ;
		}
	}
}

void	Client::concatCmdBuffer(std::string &buffer)
{
	this->cmdBuffer.append(buffer);
}

std::string	Client::getCmdBuffer(void)
{
	return (this->cmdBuffer);
}

void	Client::clearCmdBuffer(void)
{
	this->cmdBuffer.clear();
}