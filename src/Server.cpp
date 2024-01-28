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

void Server::coreProcess(void)
{
	int numOfEventsOccured;

	std::cout << "Server started on port: " << this->_port << std::endl;
	while (1)
	{
		numOfEventsOccured = poll(this->pfds.data(), this->pfds.size(), -1);
		if (numOfEventsOccured == -1)
			perror("poll system-call failed"); // not sure whether this should crash the server or not
		if ((this->pfds[0].revents & POLLIN))
		{
			Server::handleNewClient();
			numOfEventsOccured--;
		}
		if (numOfEventsOccured >= 1)
			Server::handleEstablishedClientEvents();
	}
}

void Server::handleNewClient(void)
{
	int clientSocket;

	clientSocket = accept(this->_socket, NULL, NULL);
	if (clientSocket == -1 || fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
		perror("client accepting sys calls failed"); // try to check the errno
	else
	{
		this->pfds.push_back((struct pollfd){clientSocket, POLLIN, 0});
		Client *newClient = new Client(clientSocket);
		this->_clients[clientSocket] = newClient;
		Server::sendReply("TCP connection established between you and the irc server.\n", clientSocket);
		// Server::sendReply("> ", clientSocket);
	}
}

void Server::handleEstablishedClientEvents(void)
{
	std::vector<std::string> commands;
	std::string requestMessagae;

	for (size_t i = 1; i < this->pfds.size(); i++)
	{
		if (this->pfds[i].revents & POLLIN)
		{
			if (Server::readRequest(requestMessagae, this->pfds[i].fd) == -1)
			{
				Server::closeConnection(this->pfds[i].fd);
				continue;
			}
			std::cout << this->pfds[i].fd << " sent the following message: '" << requestMessagae << "'" << std::endl; //debug
			commands = split(requestMessagae, COMMANDS_DELIMITER);
			Server::parseCommands(commands, this->pfds[i].fd);
			// Server::sendReply("> ", this->pfds[i].fd); //debug
		}
	}
}

int Server::readRequest(std::string &message, const int fd)
{
	char buf[Server::recvBufferSize];

	int bytesReceived = recv(fd, buf, Server::recvBufferSize, 0);
	if (bytesReceived == -1)
		perror("recv failed");
	else if (bytesReceived == 0)
	{
		std::cout << "Fd: "<< fd << " connection closed." << std::endl;
		return (-1);
	}
	else
	{
		buf[bytesReceived] = 0;
		message.append(buf);
		while (bytesReceived)
		{
			bytesReceived = recv(fd, buf, Server::recvBufferSize, 0);
			if (bytesReceived == -1)
			{
				if (errno != EWOULDBLOCK)
					perror("recv failed");
				return (0);
			} 
			buf[bytesReceived] = 0;
			message.append(buf);
		}
	}
	return (0);
}

void Server::parseCommands(const std::vector<std::string> commands, int clientFd)
{
	std::vector<std::string> cmd;
	Client *client = this->_clients[clientFd];

	for (size_t i = 0; i < commands.size(); i++)
	{
		commandData cmd = parseCommand(commands[i]);
		if (cmd.name == "PASS")
			passCmd(cmd, *this, *client);
		else if (cmd.name == "NICK")
			nickCmd(cmd, *this, *client);
		else if (cmd.name == "USER")
			userCmd(cmd, *client);
		else if (cmd.name == "JOIN")
			joinCmd(cmd, *this, client);
		else if (cmd.name == "PART")
			partCmd(cmd, *this, client);
		else if (cmd.name == "PRIVMSG")
			privMsgCmd(cmd, *this, *client);
		else if (cmd.name == "QUIT")
			quitCmd(cmd, *this, client);
		else
			std::cerr << "Error: invalid command: '" << commands[i] << "'" << std::endl;
	}
}

void Server::closeConnection(int fd)
{
	delete this->_clients[fd];
	this->_clients.erase(fd);
	for (std::vector<struct pollfd>::iterator it = this->pfds.begin(); it != this->pfds.end(); it++)
	{
		if (it->fd == fd)
		{
			this->pfds.erase(it);
			break;
		}
	}
}

// too many comments that should be removed later
int Server::setupServerSocket(void)
{
	int serverSocket;

	// 1. Creating socket file descriptor
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	// || fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1) // not sure if server socket should be non-blocking
		throw std::runtime_error("socket creation failed");
	// 2. Forcefully attaching socket to the port 8080

	// SOL_SOCKET flag:
	// is used for socket-level options.

	// SO_REUSEADDR flag:
	// This option allows the reuse of a local address (IP address and port number)
	// even if it's still in a TIME_WAIT state after the socket is closed.
	// This is useful when restarting a server to bind to the same address without waiting for the TIME_WAIT period to expire.
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("setsockopt failed");

	// 3.0 struct sockaddr_in setup
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(Server::getPort());

	// 3.1 binding
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("bind failed");

	// 4. listening
	// SOMAXCONN : Socket Max Connection [128] (backlog)
	if (listen(serverSocket, SOMAXCONN) < 0)
		throw std::runtime_error("listen failed");
	return (serverSocket);
}

// probably should be removed
void Server::sendReply(const std::string message, unsigned int clienFd)
{
	if (send(clienFd, message.c_str(), strlen(message.c_str()), 0) == -1)
		perror("send failed");
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

