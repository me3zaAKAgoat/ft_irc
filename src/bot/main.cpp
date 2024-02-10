#include "BMOBot.hpp"

int main(int ac, const char *av[])
{
	if (ac != 4)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password> <serverIp>" << std::endl;
		return (EXIT_FAILURE);
	}
	else if (!isValidPort(av[1]))
	{
		std::cerr << "Error: invalid port" << std::endl;
		return (EXIT_FAILURE);
	}
	else if (!isValidPassword(av[2]))
	{
		std::cerr << "Error: invalid password" << std::endl;
		return (EXIT_FAILURE);
	}
	try
	{
		BMOBot bot(atoi(av[1]), av[3]);
		bot.botRegistration(av[2]);
		bot.botCoreProcess();
		return (EXIT_SUCCESS);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error Bot: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}