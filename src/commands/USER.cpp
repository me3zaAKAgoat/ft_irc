#include "Commands.hpp"

void	user(std::vector<std::string> cmd, Client &client)
{
	// password and nickname should be set first
	if (!client.isNicknameSet())
	{
		Server::responseMsg("Nickname not set yet\r\n", client.getFd());
		return ;
	}
	if (!client.isAuthenticate())
	{
		Server::responseMsg("Password not set yet\r\n", client.getFd());
		return ;
	}
	else if (cmd.size() < 5)
	{
		Server::responseMsg(": 461 " + client.getNickname() + " USER :Not enough parameters\r\n", client.getFd());
		return ;
	}
	else if (client.isUserNameSet())
	{
		Server::responseMsg(": 462 " + client.getNickname() + " :You may not reregister\r\n", client.getFd());
		return ;
	}
	// the login/user name should be one word
	if (cmd[2] != "0" || cmd[3] != "*")
	{
		Server::responseMsg("Invalid arguments\n", client.getFd());
		return ;
	}
	// if (cmd.size() - 4 >= 2)
	// 	cmd[4] = cmd[4].erase(0, 1);  // if the login-name may contains spaces -> remove the (:) (this behavior occurs in lime chat client)
	client.setUserName(cmd[1], join((cmd.begin() + 4), cmd.end(), " "));
	Server::responseMsg(": 001 " + client.getNickname() + " :Welcome to the ftIrc Network\r\n", client.getFd());
	std::cout << "Username: " << client.getUsername() << std::endl;
	std::cout << "Realname: " << client.getRealname() << std::endl;
}
