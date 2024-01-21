#include "Commands.hpp"

bool	isValidCharNickName(char c)
{
	// check CHANTYPES also
	if (isalpha(c) || isdigit(c) \
			|| c == '[' || c == ']' \
			|| c == '{' || c == '}' \
			|| c == '\\' || c == '|' \
			|| c == ':' || c == '#')
		return (true);
	return (false);
}

bool	isValidNickName(std::vector<std::string> cmd, Client &client)
{
	if (cmd.size() > 2)
		return (Server::responseMsg(": 432 " + client.getNickName() + " <nick> :Erroneus nickname\r\n", client.getFd()), false);
	else if (isValidCharNickName(cmd[1][0]) && isdigit(cmd[1][0]))
		return (Server::responseMsg(": 432 " + client.getNickName() + " <nick> :Erroneus nickname\r\n", client.getFd()), false);
	for (size_t i = 1; i < cmd[1].size(); i++)
	{
		if (!isValidCharNickName(cmd[1][i]))
			return (Server::responseMsg(": 432 " + client.getNickName() + " <nick> :Erroneus nickname\r\n", client.getFd()), false);
	}
	return (true);
}

bool	isAlreadyUsed(std::vector<std::string> cmd, Client &client)
{
	// doesn't works yet I needs to loop through the whole nicknames of clients and check
	(void)cmd;
	return (false);
	return (Server::responseMsg(": 433 " + client.getNickName() + " <nick> :Nickname is already in use\r\n", client.getFd()), true);
}

void	NICK(std::vector<std::string> cmd, Client &client)
{
	if (!client.isAuthenticate())
		return ;
	if (cmd.size() < 2)
	{
		Server::responseMsg(": 431 " + client.getNickName() + " :No nickname given\r\n", client.getFd());
		return ;
	}
	else if (!isValidNickName(cmd, client) || isAlreadyUsed(cmd, client))
	{
		Server::responseMsg("invalid nickname OR already used\r\n", client.getFd());
		return ;
	}
	if (client.isNickNameSet())
		Server::responseMsg("; " + client.getNickName() + " changed his nickname to " + cmd[1] + ".\r\n", client.getFd()); // does not appears in lime chat (needs to modify)
	std::cout << client.getNickName() << " new NickName: " << cmd[1] << std::endl;
	client.setNickName(cmd[1]);
}