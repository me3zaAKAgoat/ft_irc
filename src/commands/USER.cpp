#include "Commands.hpp"

void		userCmd(commandData& cmd, Client &client)
{
	if (!client.isAuthenticated())
	{
		Server::sendReply("Password not set yet\r\n", client.getFd());
		return ;
	}
	if (client.getNickname().empty())
	{
		Server::sendReply("Nickname not set yet\r\n", client.getFd());
		return ;
	}
	if (cmd.arguments.size() < 4)
	{
		Server::sendReply(": 461 " + client.getNickname() + " USER :Not enough parameters\r\n", client.getFd());
		return ;
	}
	else if (!client.getUsername().empty())
	{
		Server::sendReply(": 462 " + client.getNickname() + " :You may not reregister\r\n", client.getFd());
		return ;
	}
	if (cmd.arguments[1] != "0" || cmd.arguments[2] != "*")
	{
		Server::sendReply("Invalid arguments\n", client.getFd()); // change this message to a valid irc error reply
		return ;
	}
	client.setUsername(cmd.arguments[0]);
	client.setRealname(cmd.arguments[3]);
	client._register();
	Server::sendReply(": 001 " + client.getNickname() + " :Welcome to the Irc network.\r\n", client.getFd());
}
