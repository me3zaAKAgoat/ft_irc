#include "BMObot.hpp"

int main(int ac, const char *av[])
{
	if (!isValidArgs(ac, av))
		return (EXIT_FAILURE);
	try
	{
		BMObot x(atoi(av[1]));
		x.botRegistration(av[2]);
		x.botCoreProcess();
		return (EXIT_SUCCESS);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error Bot: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}