#include "Commands.hpp"

void	PASS(std::vector<string> cmd, Client &newClient)
{
	if (cmd.size() > 2)
		cmd[1] = cmd[1].erase(0, 1); // if the password contains spaces -> remove the (:)
	newClient.setPassword(joinStrs((cmd.begin() + 1), cmd.end(), " "));
}