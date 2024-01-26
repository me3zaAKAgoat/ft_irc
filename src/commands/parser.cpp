#include "Irc.hpp"

/* needs validation and error managament
would be best if it throws errors that could be caught later*/
commandData parseCommand(const std::string &commandMessage)
{
	commandData command;

	if (commandMessage.empty())
		return command;

	bool		hasPrefix = (commandMessage[0] == ':');
	size_t		trailingPartStartPos = hasPrefix ?  commandMessage.substr(1).find(':') + 1 : commandMessage.find(':'); // substr 1 to skip the first char as it may be a prefix of the command colon which is not what we intend to manage here
	std::string middlePart = commandMessage.substr(0, trailingPartStartPos);

	std::vector<std::string> middleParams = split(middlePart, " ");
	if (!middleParams.empty())
	{
		if (hasPrefix)
			command.prefix = middleParams[0];
		command.name = middleParams[0 + (1 * hasPrefix)];
		command.arguments.insert(command.arguments.end(), middleParams.begin() + (1 + 1 * hasPrefix), middleParams.end());
	}

	if (trailingPartStartPos != std::string::npos)
	{
		std::string trailingPart = commandMessage.substr(trailingPartStartPos);
		std::vector<std::string> trailingParams = split(trailingPart, ":");
		if (!trailingParams.empty())
			command.arguments.insert(command.arguments.end(), trailingParams.begin(), trailingParams.end());
	}
	return (command);
}
