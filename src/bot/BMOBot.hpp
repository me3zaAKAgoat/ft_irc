#pragma once

#include <iomanip>
#include "Irc.hpp"
#include "response.hpp"

class BMOBot
{
public:
	BMOBot(const int port);

	void				botCoreProcess(void);
	int					readBotRequest(std::string &message, const int fd);
	void				greetAndProvideCommands(std::vector<std::string> words);
	std::string			getCmdUsage(void);
	void				botRegistration(const std::string& password);

	int getBotSocket(void);

private:
	static const int RECV_BUFFER_SIZE;

	struct		pollfd pfd;
	int			_socket;
	std::string	nickname;

};