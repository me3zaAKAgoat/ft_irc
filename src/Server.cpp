#include "Server.hpp"
#include "Commands.hpp"

const int Server::recvBufferSize = 1000;

Server::Server()
{
}

Server::Server(const int port, const std::string password)
{
	this->_port = port;
	this->_password = password;
	this->_socket = setupServerSocket();
	this->pfds.push_back((struct pollfd){this->_socket, POLLIN, 0});
}

Server::~Server(void)
{
	// Close the sockets when done
	// the order of closing affect smt or not?
	// close(Server::getClientSocket()); // close all fd-socket-clients
	
	close(this->_socket);
}

// remove the 100 message cap this function is so ass need full rehaul
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

void handleRegistrationCommand(Server &server, const std::string commands, Client &client)
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

void Server::parseCommands(const std::vector<std::string> commands, int clientFd)
{
	std::vector<std::string> cmd;
	Client *client = this->_clients[clientFd];

	for (size_t i = 0; i < commands.size(); i++)
	{
		cmd = split(commands[i], " ");
		if (isRegistrationCommand(cmd[0]))
			handleRegistrationCommand(*this, commands[i], *client);
		else
			std::cerr << "Error: invalid command" << std::endl;
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
		Server::responseMsg("Start...\n", newClient->getFd());			// debug
		Server::responseMsg("––> type a message: ", newClient->getFd()); // debug
	}
}

void Server::closeConnection(int fd)
{
	delete this->_clients[fd];
	this->_clients.erase(fd);
	std::vector<struct pollfd>::iterator it = this->pfds.begin(); 
	while (it != this->pfds.end())
	{
		if (it->fd == fd)
		{
			this->pfds.erase(it);
			break;
		}
		it++;
	}
}

void Server::handleEstablishedClientEvents(void)
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
	std::vector<std::string> commands;

	for (size_t i = 1; i < this->pfds.size(); i++)
	{

		if (this->pfds[i].revents & POLLIN)
		{
			std::string msg;

			if (Server::readRequest(msg, this->pfds[i].fd) == -1)
			{
					Server::closeConnection(this->pfds[i].fd);
					continue;
			}
			/* debug */
			std::cout << "========== NEW MSG ========== : " << msg << std::endl; // debug
			commands = split(msg, "\r\n"); // add \r if using lime chat
			Server::parseCommands(commands, this->pfds[i].fd);
			std::cout << std::endl << "========== ======= ==========" << std::endl; // debug
			Server::responseMsg("––> type a message: ", this->pfds[i].fd);
			/*		*/
		}
	}
}

void Server::coreProcess(void)
{
	int numOfEventsOccured;

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

// too many comments that should be removed later
int Server::setupServerSocket(void)
{
	int serverSocket;

	// 1. Creating socket file descriptor
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1 || fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1) // not sure if server socket should be non-blocking
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
void Server::responseMsg(const std::string message, unsigned int clienFd)
{
	if (send(clienFd, message.c_str(), strlen(message.c_str()), 0) == -1)
		perror("send failed");
}
