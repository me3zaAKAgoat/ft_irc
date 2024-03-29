#pragma once

#include "Irc.hpp"

typedef struct
{
	std::string					prefix;
	std::string					name;
	std::vector<std::string>	arguments;
} commandData;

/* all commands should take a commandData as first argument then a server and a client if needed*/
commandData	parseCommandMessage(const std::string &requestMessage);
void		nickCmd(commandData& cmd, Server &server, Client& client);
void		passCmd(commandData& cmd, Server& server, Client& client);
void		userCmd(commandData& cmd, Server& server, Client &client);
void		joinCmd(commandData& cmd, Server& server, Client& client);
void		partCmd(commandData& cmd, Server& server, Client& client);
void		privMsgCmd(commandData& cmd, Server& server, Client& client);
void		quitCmd(commandData& cmd, Server& server, Client& client);
void		topicCmd(commandData& cmd, Server& server, Client& client);
void		inviteCmd(commandData &cmd, Server &server, Client &client);
void		kickCmd(commandData& cmd, Server &server, Client &client);
void		modeCmd(commandData& cmd, Server &server, Client &client);
void		noticeCmd(commandData& cmd, Server& server, Client& client);
std::string removeTrailingCRLF(const std::string& input);
