#pragma once

#include "Irc.hpp"

class BMOBot
{
public:
	BMOBot(const int port);

	void botCoreProcess(void);
	int readBotRequest(std::string &message, const int fd);

	int getBotSocket(void);

private:
	static const int RECV_BUFFER_SIZE;
	struct pollfd pfd;
	int _socket;
};