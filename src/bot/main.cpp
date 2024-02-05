#include "BMOBot.hpp"

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

	if (portNbr < 0 || portNbr > 65535) // 65535 is the max port number in tcp/udp
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
	try
	{
		BMOBot x(atoi(av[1]));
		x.botRegistration(av[2]);
		x.botCoreProcess();
		return (EXIT_SUCCESS);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error Bot: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
}