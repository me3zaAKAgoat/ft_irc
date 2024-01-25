#include "Commands.hpp"

bool isWhiteSpace(char c)
{
	if (c == 32 || (c >= 9 && c <= 11))
		return (true);
	return (false);
}

std::string extractPassword(std::string cmd)
{
	std::string result;
	size_t i = 0;

	cmd = cmd.substr(cmd.find("PASS") + strlen("PASS"));
	while (isWhiteSpace(cmd[i]))
		i++;
	while (i < cmd.size())
		result.push_back(cmd[i++]);
	return (result);
}

void pass(Server& server, std::string cmd, Client &client, size_t numArgs)
{
	// // if (cmd.size() > 2)
	// 	cmd[1] = cmd[1].erase(0, 1); // if the password contains spaces -> remove the (:) (this behavior occurs in lime chat client)
	if (numArgs < 2)
		Server::responseMsg(": 461 " + client.getNickname() + " PASS :Not enough parameters\r\n", client.getFd());
	else if (client.isAuthenticated())
	{
		if (!client.getNickname().empty())
			Server::responseMsg(": 462 " + client.getNickname() + " :You may not reregister\r\n", client.getFd());
		else
			Server::responseMsg(": 462 " + std::string("*no nickname*") + " :You may not reregister\r\n", client.getFd());
	}
	else if (extractPassword(cmd) != server.getPassword())
	{
		std::cout << "password receive from client: '" << extractPassword(cmd) << "'" << std::endl;
		Server::responseMsg(": 464 " + client.getNickname() + " :Password incorrect\r\n", client.getFd());
	}
	else
	{
		client.setPassword(extractPassword(cmd));
		client.authenticate();
		std::cout << "Password is correct: client: " << client.getFd() << std::endl;
	}
}