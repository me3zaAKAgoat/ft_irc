#include "Commands.hpp"

void	NICK(std::vector<string> cmd, Client &newClient)
{
	// check if the nickname is alredy used
	if (cmd.size() > 2)
		cmd[1] = cmd[1].erase(0, 1); // if the nickName contains spaces -> remove the (:)
	newClient.setNickName(joinStrs((cmd.begin() + 1), cmd.end(), " ")); // I think the nickname should be a name without spaces
}