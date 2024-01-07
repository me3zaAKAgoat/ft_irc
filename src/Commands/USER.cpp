#include "Commands.hpp"

void	USER(std::vector<string> cmd, Client &newClient)
{
	std::vector<string>::iterator it = cmd.begin();
	size_t i = 0;
	while (it != cmd.end())
	{
		if (*it == "0")
			break;
		it++;
		i++;
	}
	newClient.setLoginName(joinStrs((cmd.begin() + 1), it, " "));
	it++; // skip the "0" host-name
	i++;
	it++; // skip the "*" server-name
	i++;
	if (cmd.size() - i >= 2)
		cmd[i] = cmd[i].erase(0, 1);  // if the password contains spaces -> remove the (:)
	newClient.setRealName(joinStrs((cmd.begin() + i), cmd.end(), " "));
}