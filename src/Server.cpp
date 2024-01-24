#include "Server.hpp"
#include "Commands.hpp"

Server::Server()
{
}

Server::Server(const int port, const std::string password)
{
	this->_port = port;
	this->_password = password;
	this->_socket = setupServerSocket();	
}

Server::~Server(void)
{
	// Close the sockets when done
	// the order of closing affect smt or not?
	// close(Server::getClientSocket()); // close all fd-socket-clients
	close(this->_socket);
}

// remove the 100 message cap this function is so ass need full rehaul
bool Server::ReceiveRequest(std::string &message, const int fd)
{
	char msg[100];

	int bytesReceived = recv(fd, msg, sizeof(msg), 0);
	if (bytesReceived == -1)
		return (perror("recv failed"), false);
	else if (bytesReceived == 0)
	{
		perror("the remote side has closed the connection on you!"); // ur program is not talking to anyone it just facilitating communication between clients
		return (false);
	}
	message.clear();
	msg[bytesReceived] = '\0';
	message = msg;
	return (true);
}

void handleRegistrationCommand(Server& server, const std::string commands, Client &client)
{
	std::vector<std::string> cmdSplit;

	cmdSplit = split(commands, " ");
	if (cmdSplit[0] == "PASS")
		pass(server, commands, client, cmdSplit.size());
	else if (cmdSplit[0] == "NICK")
		nick(cmdSplit, client);
	else if (cmdSplit[0] == "USER")
		user(cmdSplit, client);
}

bool isRegistrationCommand(std::string cmd)
{
	return (cmd == "PASS" || cmd == "NICK" || cmd == "USER");
}

void Server::parseCommands(const std::vector<std::string> commands, unsigned int clientIndex)
{
	std::vector<std::string> cmd;
	std::vector<Client>::iterator client = this->_clients.begin() + clientIndex; // why not have a simple [] accessor?

	for (size_t i = 0; i < commands.size(); i++)
	{
		cmd = split(commands[i], " ");
		if (isRegistrationCommand(cmd[0]))
			handleRegistrationCommand(*this, commands[i], (*client)); // this->_clients[clientIndex]
		else // testing should be removed later
			std::cout << "invalid command" << std::endl;
	}
	std::cout << "msg from client-" << clientIndex + 1 << "-" << this->_clients[clientIndex].getNickname() << std::endl;
}

void	Server::handleNewClient(void)
{
	int		clientSocket;

	clientSocket = accept(this->_socket, NULL, NULL);
	if (clientSocket == -1)
		perror("accept system-call failed"); // try to check the errno
	else
	{
		Server::pushBackFds(clientSocket);
		Client	newClient(clientSocket);
		this->_clients.push_back(newClient);
		// std::cout << "Connection established with a client: " << clientSocket - 3 << std::endl;
		Server::responseMsg("Start...\n", newClient.getFd());
		Server::responseMsg("––> type a message: ", newClient.getFd());
	}
}


void	Server::handleEstablishedClientEvents(void)
{
	// POLLERR;
	// POLLHUP;
	// POLLIN;
	// POLLNVAL;
	// POLLOUT;
	// POLLPRI;
	// POLLRDBAND;
	// POLLRDNORM;
	// POLLWRBAND;
	// POLLWRNORM;
	std::vector<std::string>	commands;

	for (size_t i = 1; i < this->sizePfds; i++)
	{
		if (this->pfds[i].revents & POLLIN)
		{
			std::string msg;
			if (!Server::ReceiveRequest(msg, this->pfds[i].fd))
			{
				// if (Server::pfds[i].fd != Server::getServerSocket())
				// {
					this->_clients.pop_back();
					Server::removeFdClient(this->pfds[i].fd);
					continue;
				// }
			}
			std::cout << "========== NEW MSG ========== : " << msg << std::endl;
			commands = split(msg, "\n"); // add \r if using lime chat
			Server::parseCommands(commands, i - 1);
			std::cout << std::endl << "========== ======= ==========" << std::endl;
			Server::responseMsg("––> type a message: ", this->pfds[i].fd);
		}
	}
}

void	Server::coreProcess(void)
{
	int	numOfEventsOccured;

	while (1)
	{
		numOfEventsOccured = poll(this->pfds, this->sizePfds, -1);
		if (numOfEventsOccured == -1)
			perror("poll system-call failed"); // not sure whether this should crash the server or not
		if (numOfEventsOccured >= 1 && (this->pfds[0].revents & POLLIN)) // this could cause a bug because of the first rehaul of this code
			Server::handleNewClient();
		if (numOfEventsOccured > 1 && (this->pfds[0].revents & POLLIN))
			Server::handleEstablishedClientEvents();
	}
}

// too many comments that should be removed later
int Server::setupServerSocket(void)
{
	int server_socket;

	// 1. Creating socket file descriptor
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
		throw std::runtime_error("socket creation failed");
	// 2. Forcefully attaching socket to the port 8080

	// SOL_SOCKET flag:
	// is used for socket-level options.

	// SO_REUSEADDR flag:
	// This option allows the reuse of a local address (IP address and port number)
	// even if it's still in a TIME_WAIT state after the socket is closed.
	// This is useful when restarting a server to bind to the same address without waiting for the TIME_WAIT period to expire.
	int opt = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("setsockopt failed");

	// 3.0 struct sockaddr_in setup
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(Server::getPort());

	// 3.1 binding
	if (bind(server_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("bind failed");

	// 4. listening
	// SOMAXCONN : Socket Max Connection [128] (backlog)
	if (listen(server_socket, SOMAXCONN) < 0)
		throw std::runtime_error("listen failed");
	return (server_socket);
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

// probably should be removed
void Server::responseMsg(const std::string message, unsigned int fdClient)
{
	if (send(fdClient, message.c_str(), strlen(message.c_str()), 0) == -1)
		perror("send failed");
}

void Server::pushBackFds(const int fd)
{
	struct pollfd *newFds;

	newFds = new struct pollfd[Server::sizePfds + 1];
	size_t i = 0;
	while (i < Server::sizePfds)
	{
		newFds[i].fd = Server::pfds[i].fd;
		newFds[i].events = Server::pfds[i].events;
		i++;
	}
	if (fd < 0)
		std::cout << "pushBackFds failed: " << fd << std::endl;
	else
	{
		newFds[i].fd = fd;
		newFds[i].events = POLLIN;
	}
	if (Server::sizePfds > 0)
		delete[] (Server::pfds);
	Server::sizePfds++;
	Server::pfds = newFds;
}

void	Server::removeFdClient(const int fd)
{
	struct pollfd *newFds;

	newFds = new struct pollfd[Server::sizePfds - 1];
	size_t i = 0;
	size_t j = 0;
	while (i < Server::sizePfds)
	{
		if (Server::pfds[i].fd != fd)
		{
			newFds[j].fd = Server::pfds[i].fd;
			newFds[j].events = POLLIN;
			j++;
		}
		i++;
	}
	std::cout << "remove fd: " << fd << std::endl;
	close(fd);
	delete[] (Server::pfds);
	Server::sizePfds--;
	Server::pfds = newFds;
}
