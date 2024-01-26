#include "Irc.hpp"

bool isValidPassword(const std::string password)
{
	if (password.empty())
		return (std::cerr << "Error: password argument empty!" << std::endl, false);
	return (true);
}

bool isValidPort(const std::string port)
{
	if (port.empty())
		return (std::cerr << "Error: port argument empty!" << std::endl, false);

	int i = 0;
	while (port[i])
	{
		if (!isdigit(port[i]))
			return (std::cerr << "Error: port should be a number" << std::endl, false);
		i++;
	}

	int portNbr = std::atoi(port.c_str());

	if (portNbr < 0 || portNbr > 65535)
		return (std::cerr << "Error: Port out of valid range (0-65535)!" << std::endl, false);

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
	try {
		Server server(std::atof(av[1]), av[2]);

		server.coreProcess();
		return (EXIT_SUCCESS);
	}
	catch( const std::exception & e ) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch( ... ) {
		std::cerr << "Error: Unknown exception caught" << std::endl;
	}
	return (EXIT_FAILURE);
}
