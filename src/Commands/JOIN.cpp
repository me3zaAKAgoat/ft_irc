#include "Commands.hpp"

bool	ConsecutiveChar(std::string str, char c)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == c)
		{
			if (str[i + 1] == c)
				return (std::cout << "Error: consecutive '" << c << "'" << std::endl, false);
		}
	}
	return (true);
}

void	JOIN(std::vector<std::string> cmd, Client &client)
{
	std::vector<std::string>::iterator	it = cmd.begin() + 1;
	std::vector<std::string>			channelNames;
	std::vector<std::string>			channelKeys;

	std::cout << "Enter to JOIN func" << std::endl;
	if (cmd.size() != 2 && cmd.size() != 3)
		Server::responseMsg(": 461 " + client.getNickName() + " JOIN :Not enough parameters\r\n", client.getFd());
	if (ConsecutiveChar(*(it), ','))
		std::vector<std::string> channelNames = ft_split(*(it), ",");
	else
		return ;
	if ((it + 1) != cmd.end())
	{
		if (ConsecutiveChar(*(it + 1), ','))
			std::vector<std::string> channelKeys = ft_split(*(it + 1), ",");
		else
			return ;
	}
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		if (channelNames[i][0] != '#' && channelNames[i][0] != '&')
			Server::responseMsg(": 476 " + channelNames[i] + " :Bad Channel Mask\r\n", client.getFd());
		if (i < channelKeys.size())
			std::cout << client.getNickName() << " try to join the channel: " << channelNames[i] << " with the key " << channelKeys[i] << std::endl;
		else
			std::cout << client.getNickName() << " try to join the channel: " << channelNames[i] << std::endl;
	}	
}
