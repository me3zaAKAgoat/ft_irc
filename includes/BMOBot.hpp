#pragma once

#include <iomanip>
#include "Irc.hpp"

class BMObot
{
public:
	BMObot(const int port);

	void		botRegistration(const std::string &password) const;
	void		botCoreProcess(void);
	int			readBotRequest(std::string &message, const int fd);

	void		greetAndProvideCommands(std::string clientNickname);
	void		handleDateCmd(std::string& clientNickname);
	void		handleUsageCmd(std::string& clientNickname);
	void		invalidCmd(const commandData& cmd);
	std::string	getCmdsUsage(void);
	void		cleanseCommandData(commandData& cmd);
	void		commandProcess(commandData& cmd);

	int getBotSocket(void) const;

private:
	static const int RECV_BUFFER_SIZE;

	struct pollfd	pfd;
	int				_socket;
	std::string		nickname;
};