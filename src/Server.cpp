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

std::string joinStrs(std::vector<std::string>::iterator itBegin, std::vector<std::string>::iterator itEnd, std::string separator = "")
{
	std::string result;

	if (itBegin == itEnd)
		return (*itBegin);
	while (itBegin != itEnd)
	{
		result += *itBegin;
		if ((itBegin + 1) != itEnd)
			result += separator;
		itBegin++;
	}
	return (result);
}

void Server::addClient(Client &newClient)
{
	std::cout << "password is correct - client added" << std::endl;
	this->_clients.push_back(newClient);
}

bool Server::ReceiveRequest(std::string &message, const int fd)
{
	char msg[100];

	int bytesReceived = recv(fd, msg, sizeof(msg), 0);
	if (bytesReceived == -1)
		return (perror("recv failed"), false);
	else if (bytesReceived == 0)
	{
		perror("the remote side has closed the connection on you!");
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
		PASS(server, commands, client, cmdSplit.size());
	else if (cmdSplit[0] == "NICK")
		NICK(cmdSplit, client);
	else if (cmdSplit[0] == "USER")
		USER(cmdSplit, client);
}

bool isRegistrationCommand(std::string cmd)
{
	if (cmd == "PASS" || cmd == "NICK" || cmd == "USER")
		return (true);
	return (false);
}

void Server::parseCommands(const std::vector<std::string> commands, unsigned int clientIndex)
{
	std::vector<std::string> cmd;
	std::vector<Client>::iterator client = this->_clients.begin() + clientIndex;
	for (size_t i = 0; i < commands.size(); i++)
	{
		cmd = split(commands[i], " ");
		if (isRegistrationCommand(cmd[0]))
			handleRegistrationCommand(*this, commands[i], (*client));
		else
			std::cout << "invalid command" << std::endl;
	}
	std::cout << "msg from client-" << clientIndex + 1 << "-" << this->_clients[clientIndex].getNickName() << std::endl;
}

// what if there is multiple spaces mtab3ine
// this function is really vulnerable its not done yet
std::vector<std::string> split(const std::string &input, const std::string &separator)
{
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t found = input.find(separator);
	if (found == std::string::npos)
	{
		if (separator != " ")
			std::cout << "separator doesn't found" << std::endl;
		result.push_back(input);
		return (result);
	}
	while (found != std::string::npos)
	{
		result.push_back(input.substr(start, found - start));
		start = found + separator.size(); // Move past the separator
		found = input.find(separator, start);
	}
	if (input.substr(start) != "")
		result.push_back(input.substr(start));
	return (result);
}

void	Server::acceptNewConnection(void)
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


void	Server::detectEventInClientsFds(void)
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

	for (size_t i = 1; i < Server::sizeFds; i++)
	{
		if (Server::fds[i].revents & POLLIN)
		{
			std::string msg;
			if (!Server::ReceiveRequest(msg, fds[i].fd))
			{
				// if (Server::fds[i].fd != Server::getServerSocket())
				// {
					this->_clients.pop_back();
					Server::removeFdClient(Server::fds[i].fd);
					continue;
				// }
			}
			std::cout << "========== NEW MSG ========== : " << msg << std::endl;
			commands = split(msg, "\n"); // add \r if using lime chat
			Server::parseCommands(commands, i - 1);
			std::cout << std::endl << "========== ======= ==========" << std::endl;
			Server::responseMsg("––> type a message: ", Server::fds[i].fd);
		}
		// else
		// 	std::cout << "event does not occurs at client: " << Server::fds[i].fd << std::endl;
	}
}

void	Server::coreProcess(void)
{
	int	numOfEventsOccured;

	while (1)
	{
		numOfEventsOccured = poll(this->fds, this->sizeFds, -1);
		if (numOfEventsOccured == -1)
			perror("poll system-call failed"); // not sure whether this should crash the server or not
		if (numOfEventsOccured >= 1 && (this->fds[0].revents & POLLIN)) // this could cause a bug because of the first rehaul of this code
			Server::acceptNewConnection();
		if (numOfEventsOccured > 1 && (this->fds[0].revents & POLLIN))
			Server::detectEventInClientsFds();
	}
}

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
	return (Server::_port);
}

int Server::getServerSocket(void)
{
	return (Server::_socket);
}

std::string Server::getPassword(void)
{
	return (Server::_password);
}

void Server::setPort(const int port)
{
	Server::_port = port;
}

void Server::setServerSocket(const int serverSocket)
{
	Server::_socket = serverSocket;
}

void Server::setPassword(const std::string password)
{
	Server::_password = password;
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

	newFds = new struct pollfd[Server::sizeFds + 1];
	size_t i = 0;
	while (i < Server::sizeFds)
	{
		newFds[i].fd = Server::fds[i].fd;
		newFds[i].events = Server::fds[i].events;
		i++;
	}
	if (fd < 0)
		std::cout << "pushBackFds failed: " << fd << std::endl;
	else
	{
		newFds[i].fd = fd;
		newFds[i].events = POLLIN;
	}
	if (Server::sizeFds > 0)
		delete[] (Server::fds);
	Server::sizeFds++;
	Server::fds = newFds;
}

void	Server::removeFdClient(const int fd)
{
	struct pollfd *newFds;

	newFds = new struct pollfd[Server::sizeFds - 1];
	size_t i = 0;
	size_t j = 0;
	while (i < Server::sizeFds)
	{
		if (Server::fds[i].fd != fd)
		{
			newFds[j].fd = Server::fds[i].fd;
			newFds[j].events = POLLIN;
			j++;
		}
		i++;
	}
	std::cout << "remove fd: " << fd << std::endl;
	close(fd);
	delete[] (Server::fds);
	Server::sizeFds--;
	Server::fds = newFds;
}