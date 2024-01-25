#include "Commands.hpp"

void	userCmd(std::vector<std::string> cmd, Client &client)
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
	else if (cmd.size() < 5)
	{
		Server::sendResponse(": 461 " + client.getNickname() + " USER :Not enough parameters\r\n", client.getFd());
		return ;
	}
	else if (!client.getUsername().empty())
	{
		Server::sendResponse(": 462 " + client.getNickname() + " :You may not reregister\r\n", client.getFd());
		return ;
	}
	// the login/user name should be one word
	if (cmd[2] != "0" || cmd[3] != "*")
	{
		Server::sendResponse("Invalid arguments\n", client.getFd());
		return ;
	}
	// if (cmd.size() - 4 >= 2)
	// 	cmd[4] = cmd[4].erase(0, 1);  // if the login-name may contains spaces -> remove the (:) (this behavior occurs in lime chat client)
	client.setUsername(cmd[1]);
	client.setRealname(join((cmd.begin() + 4), cmd.end(), " "));
	Server::sendResponse(": 001 " + client.getNickname() + " :Welcome to the irc Network\r\n", client.getFd());
	std::cout << "Username: " << client.getUsername() << std::endl;
	std::cout << "Realname: " << client.getRealname() << std::endl;
}
