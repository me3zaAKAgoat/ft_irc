#include "Irc.hpp"

int main(int argc, const char *argv[])
{
	std::vector<std::string> commands;
	std::vector<std::string> cmd;
	std::string message;

	if (!isValidArgs(argc, argv))
		return (EXIT_FAILURE);
	Server::setPassword(argv[1]);
	Server::setPort(std::atoi(argv[2]));
	Server::setupServerSocket();
	Server::process();
	Server::cleanupResources();
	return (EXIT_SUCCESS);
}
