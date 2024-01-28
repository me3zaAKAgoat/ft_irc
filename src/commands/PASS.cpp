#include "Commands.hpp"

void passCmd(commandData& cmd, Server& server, Client& client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(": 461 " + client.getNickname() + " PASS :Not enough parameters\r\n", client.getFd());
		return ;
	}
	if (client.isAuthenticated())
	{
		Server::sendReply(": 462 " + !client.getNickname().empty() ? client.getNickname() : std::string("*")\
		+ " :You may not reregister\r\n", client.getFd());
		return ;
	}
	if (cmd.arguments[0] == server.getPassword())
		client.authenticate();
	else
		Server::sendReply(": 464 " + client.getNickname() + " :Password incorrect\r\n", client.getFd());
}