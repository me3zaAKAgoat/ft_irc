#pragma once

#include "Irc.hpp"
#include <iomanip>
#include <sstream>
#include <arpa/inet.h>

class BMOBot
{
	public:
		BMOBot(const int port, std::string serverIp);
		~BMOBot(void);

		void		botRegistration(const std::string &password) const;
		void		botCoreProcess(void);
		int			readBotRequest(std::string &message, const int fd);

		void		greetAndProvideCommands(std::string clientNickname);
		void		handleDateCmd(std::string& clientNickname);
		void		handleUsageCmd(std::string& clientNickname);
		void		invalidCmd(const std::string& commandName, const std::string& clientNickname);
		std::string	getCmdsUsage(void);
		void		processCommand(commandData& cmd);

		int getBotSocket(void) const;

	private:
		static const int RECV_BUFFER_SIZE;

		struct pollfd	pfd;
		int				_socket;
		std::string		nickname;
		std::string		username;
		std::string		serverIp;
};