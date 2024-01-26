#include "Commands.hpp"

void passCmd(commandData& cmd, Server& server, Client& client)
{
	if (!cmd.arguments.size())
	{
		Server::sendResponse(": 461 " + client.getNickname() + " PASS :Not enough parameters\r\n", client.getFd());
		return ;
	}
	if (client.isAuthenticated())
	{
		Server::sendResponse(": 462 " + !client.getNickname().empty() ? client.getNickname() : std::string("*no nickname*")\
		+ " :You may not reregister\r\n", client.getFd());
		return ;
	}
	if (cmd.arguments[0] == server.getPassword())
		client.authenticate();
	else
		Server::sendResponse(": 464 " + client.getNickname() + " :Password incorrect\r\n", client.getFd());
}