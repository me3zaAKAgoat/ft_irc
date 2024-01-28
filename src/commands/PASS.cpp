#include "Commands.hpp"

void passCmd(commandData& cmd, Server& server, Client& client)
{
	if (!cmd.arguments.size())
	{
		Server::sendReply(ERR_NEEDMOREPARAMS(client.getNickname(), cmd.name), client.getFd());
		return ;
	}
	if (client.isAuthenticated())
	{
		Server::sendReply(ERR_ALREADYREGISTERED(!client.getNickname().empty() ? client.getNickname() : std::string("*")), client.getFd());
		return ;
	}
	if (cmd.arguments[0] == server.getPassword())
		client.authenticate();
	else
		Server::sendReply(ERR_PASSWDMISMATCH(client.getNickname()), client.getFd());
}