#include "Server.hpp"
#include "Commands.hpp"

const int Server::recvBufferSize = 1000;

Server::Server(const int port, const std::string password)
{
	this->_port = port;
	this->_password = password;
	this->_socket = setupServerSocket();
	this->pfds.push_back((struct pollfd){this->_socket, POLLIN, 0});
}

Server::~Server(void)
{
	for (std::map<int, Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		delete it->second;
		close(it->first);
	}
	close(this->_socket);
}

typedef void (*CommandHandler)(commandData&, Server&, Client&);

bool requiresRegistration(const std::string& cmdName) {
	std::string tmp[] = {"PASS", "NICK", "USER"}; /* disgusting way to initalize a set */
    static std::set<std::string> authCommands(tmp, tmp + sizeof(tmp) / sizeof(tmp[0]));
    return !(authCommands.count(cmdName));
}

bool isValidCommand(std::map<std::string, CommandHandler> commandHandlers , const std::string& cmdName) {
	return commandHandlers.count(cmdName);
}

void Server::parseCommands(const std::vector<std::string> commands, int clientFd)
{
	Client *client = this->_clients[clientFd];
	std::map<std::string, CommandHandler> commandHandlers;
	commandHandlers["PASS"] = passCmd;
	commandHandlers["NICK"] = nickCmd;
	commandHandlers["USER"] = userCmd;
	commandHandlers["JOIN"] = joinCmd;
	commandHandlers["PART"] = partCmd;
	commandHandlers["PRIVMSG"] = privMsgCmd;
	commandHandlers["QUIT"] = quitCmd;
	commandHandlers["KICK"] = kickCmd;

	for (size_t i = 0; i < commands.size(); i++)
	{
		commandData cmd = parseCommandMessage(commands[i]);

		if (!isValidCommand(commandHandlers, cmd.name))
		{
			Server::sendReply(ERR_UNKNOWNCOMMAND(client->getNickname(), cmd.name), clientFd);
			continue;
		}
		if (requiresRegistration(cmd.name) && !client->isRegistered())
		{
			Server::sendReply(ERR_NOTREGISTERED(std::string("*")), clientFd);
			continue;
		}
		commandHandlers[cmd.name](cmd, *this, *client);
	}
}

void	Server::addChannel(Channel *channel)
{
	this->_channels.push_back(channel);
}

void	Server::removeChannel(Channel *channel)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i] == channel)
		{
			delete this->_channels[i];
			this->_channels.erase(this->_channels.begin() + i);
			return ;
		}
	}
}

std::map<int, Client *>		Server::getClients(void)
{
	return (this->_clients);
}

std::vector<Channel *>		Server::getChannels(void)
{
	return (this->_channels);
}

int Server::getPort(void)
{
	return (this->_port);
}

int Server::getServerSocket(void)
{
	return (this->_socket);
}

std::string Server::getPassword(void)
{
	return (this->_password);
}

void Server::setPort(const int port)
{
	this->_port = port;
}

void Server::setServerSocket(const int serverSocket)
{
	this->_socket = serverSocket;
}

void Server::setPassword(const std::string password)
{
	this->_password = password;
}

Channel* Server::getChannelByName(const std::string name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (this->_channels[i]->getName() == name)
			return (this->_channels[i]);
	}
	return (NULL);
}

Client*	Server::getClientByNickname(const std::string nickname)
{
	for (std::map<int, Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
	}
	return (NULL);
}