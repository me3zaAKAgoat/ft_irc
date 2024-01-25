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

std::vector<std::string> split(const std::string &input, const std::string &separator)
{
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t found = input.find(separator);

	while (found != std::string::npos)
	{
		result.push_back(input.substr(start, found - start));
		start = found + separator.size(); // Move past the separator
		found = input.find(separator, start);
	}

	// Push the last part of the string if it's not empty
	std::string lastPart = input.substr(start);
	if (!lastPart.empty())
		result.push_back(lastPart);
	return result;
}

std::string join(StringVectorIterator itBegin, StringVectorIterator itEnd, const std::string& separator = "")
{
	if (itBegin == itEnd) {
		return "";
	}

	std::ostringstream result;
	result << *itBegin;

	for (StringVectorIterator it = itBegin + 1; it != itEnd; ++it) {
		result << separator << *it;
	}

	return result.str();
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
