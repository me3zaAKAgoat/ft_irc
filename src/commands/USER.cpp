#include "Commands.hpp"

void		userCmd(commandData& cmd, Client &client)
{
	// password and nickname should be set first
	if (client.getNickname().empty())
	{
		Server::sendResponse("Nickname not set yet\r\n", client.getFd());
		return ;
	}
	if (!client.isAuthenticated())
	{
		Server::sendResponse("Password not set yet\r\n", client.getFd());
		return ;
	}
	if (cmd.arguments.size() < 4)
	{
		Server::sendResponse(": 461 " + client.getNickname() + " USER :Not enough parameters\r\n", client.getFd());
		return ;
	}
	else if (!client.getUsername().empty())
	{
		Server::sendResponse(": 462 " + client.getNickname() + " :You may not reregister\r\n", client.getFd());
		return ;
	}
	if (cmd.arguments[1] != "0" || cmd.arguments[2] != "*")
	{
		Server::sendResponse("Invalid arguments\n", client.getFd()); // what the fuck is this message
		return ;
	}
	client.setUsername(cmd.arguments[0]);
	client.setRealname(cmd.arguments[3]);
	Server::sendResponse(": 001 " + client.getNickname() + " :Welcome to the Irc network.\r\n", client.getFd());
}
