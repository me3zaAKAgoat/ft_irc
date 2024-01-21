#include "Commands.hpp"

void	PASS(std::vector<std::string> cmd, Client &client)
{
	// // if (cmd.size() > 2)
	// 	cmd[1] = cmd[1].erase(0, 1); // if the password contains spaces -> remove the (:) (this behavior occurs in lime chat client)
	if (client.isAuthenticate())
		Server::responseMsg(": 462 " + client.getNickName() + " :You may not reregister\r\n", client.getFd());
	else if (joinStrs((cmd.begin() + 1), cmd.end(), " ") != Server::getPassword())
		Server::responseMsg(": 464 " + client.getNickName() + " :Password incorrect\r\n", client.getFd());
		// std::cout << "password recieve from client: " << joinStrs((cmd.begin() + 1), cmd.end(), " ") << std::endl;
	else
	{
		client.setPassword(joinStrs((cmd.begin() + 1), cmd.end(), " "));
		client.authenticate();
	}
}