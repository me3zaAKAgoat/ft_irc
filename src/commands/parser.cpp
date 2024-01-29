#include "Irc.hpp"

/*
vunlerable and needs more testing
*/
commandData parseCommandMessage(const std::string &commandMessage)
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

		size_t startIdx = static_cast<size_t>(hasPrefix);

		if (middleParams.size() > startIdx)
		{
			command.name = middleParams[startIdx];
			command.arguments.insert(command.arguments.end(), middleParams.begin() + startIdx + 1, middleParams.end());
		}
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
