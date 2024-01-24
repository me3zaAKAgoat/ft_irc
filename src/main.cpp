#include "Irc.hpp"

bool isValidPassword(const std::string password)
{
	if (password == "")
		return (std::cerr << "Error: password argument empty!" << std::endl, false);
	return (true);
}

bool isValidPort(const std::string port)
{
	int i = 0;
	if (port == "")
		return (std::cerr << "Error: port argument empty!" << std::endl, false);
	while (port[i])
	{
		if (!isdigit(port[i]))
			return (perror("port should be a number\n"), false);
		i++;
	}
	return (true);
}

bool isValidArgs(int ac, const char *av[])
{
	if (ac != 3)
		return (std::cerr << "Error: wrong number of arguments" << std::endl, false);
	else if (!isValidPort(av[1]))
		return (std::cerr << "Error: invalid port" << std::endl, false);
	else if (!isValidPassword(av[2]))
		return (std::cerr << "Error: invalid password" << std::endl, false);
	return true;
}

int main(int ac, const char *av[])
{
	if (!isValidArgs(ac, av))
		return (EXIT_FAILURE);

	Server server(std::atof(av[1]), av[2]);

	/* skipped rehaul of this*/
	std::vector<std::string> commands;
	std::vector<std::string> cmd;
	std::string message;
	/*						*/
	server.coreProcess();
	return (EXIT_SUCCESS);
}
