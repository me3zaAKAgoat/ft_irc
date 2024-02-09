#include "Irc.hpp"

std::vector<std::string> split(const std::string &input, const std::string &separator)
{
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t found = input.find(separator);

	while (found != std::string::npos)
	{
		if (found != start) // Only push non-empty substrings
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

std::string join(StringVectorIterator itBegin, StringVectorIterator itEnd, const std::string& separator)
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

std::string join(std::vector<std::string> strings, const std::string& separator)
{
	return join(strings.begin(), strings.end(), separator);
}

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
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return (false);
	}
	else if (!isValidPort(av[1]))
	{
		std::cerr << "Error: invalid port" << std::endl;
		return (false);
	}
	else if (!isValidPassword(av[2]))
	{
		std::cerr << "Error: invalid password" << std::endl;
		return (false);
	}
	return true;
}

std::string formulatePrefix(std::string hostname, std::string nickname, std::string username)
{
	return (":" + nickname + "!" + username + "@" + hostname);
}

std::string removeTrailingCRLF(const std::string& input) {
    size_t length = input.length();

    // Check if the string ends with '\r\n'
    if (length >= 2 && input[length - 2] == '\r' && input[length - 1] == '\n')
	{
        return input.substr(0, length - 2); // Remove the last two characters
	}
	return input; // No trailing '\r\n', return the original string
}
