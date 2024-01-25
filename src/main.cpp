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

// what if there is multiple spaces mtab3ine
// this function is really vulnerable its not done yet
std::vector<std::string> split(const std::string &input, const std::string &separator)
{
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t found = input.find(separator);

	if (found == std::string::npos)
	{
		if (separator != " ")
			std::cout << "separator doesn't found" << std::endl;
		result.push_back(input);
		return (result);
	}
	while (found != std::string::npos)
	{
		result.push_back(input.substr(start, found - start));
		start = found + separator.size(); // Move past the separator
		found = input.find(separator, start);
	}
	if (input.substr(start) != "")
		result.push_back(input.substr(start));
	return (result);
}

std::string join(std::vector<std::string>::iterator itBegin, std::vector<std::string>::iterator itEnd, std::string separator = "")
{
	std::string result;

	if (itBegin == itEnd)
		return (*itBegin);
	while (itBegin != itEnd)
	{
		result += *itBegin;
		if ((itBegin + 1) != itEnd)
			result += separator;
		itBegin++;
	}
	return (result);
}

int main(int ac, const char *av[])
{
	if (!isValidArgs(ac, av))
		return (EXIT_FAILURE);
	try {
		Server server(std::atof(av[1]), av[2]);

		/* skipped rehaul of this*/
		std::vector<std::string> commands;
		std::vector<std::string> cmd;
		std::string message;
		/*						*/
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
