#include "Commands.hpp"

void passCmd(commandData& cmd, Client& client)
{
	if (!cmd.arguments.size())
	{
		Server::sendResponse(": 461 " + client.getNickname() + " PASS :Not enough parameters\r\n", client.getFd());
		return ;
	}
	if (client.isAuthenticated())
	{
		Server::sendResponse(": 462 " + !client.getNickname().empty() ? client.getNickname() : std::string("*no nickname*") + " :You may not reregister\r\n", client.getFd());
		return ;
	}
	client.setPassword(cmd.arguments[0]);
	client.authenticate();
}