#include "Irc.hpp"

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
