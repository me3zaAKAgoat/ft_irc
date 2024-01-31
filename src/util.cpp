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