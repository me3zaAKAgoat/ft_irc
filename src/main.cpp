#include "Irc.hpp"

int main(int argc, const char *argv[])
{
	std::vector<string> commands;
	std::vector<string> cmd;
	string message;

	if (!isValidArgs(argc, argv))
		return (EXIT_FAILURE);
	Server::setPassword(argv[1]);
	Server::setPort(std::atoi(argv[2]));

	setupSocket();

	Client newClient;
	while (1)
	{
		if (!Server::ReceiveRequest(message))
			break;
		std::cout << "========== NEW MSG ==========" << std::endl;
		std::cout << message << std::endl;
		commands = ft_split(message, "\r\n");
		Server::parseCommands(commands, newClient);
		if (Server::getPassword() != newClient.getPassword() && newClient.getNickName() != "")
		{
			string message = ": 464 " + newClient.getNickName() + " :Password incorrect\r\n";
			Server::responseMsg(message);
		}
		else if (newClient.getNickName() != "" && newClient.getLoginName() != "" && newClient.getRealName() != "")
			Server::addClient(newClient);
	}

	cleanupResources();

	return (EXIT_SUCCESS);
}
