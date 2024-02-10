#include "Server.hpp"

void Server::handleNewClient(void)
{
	int clientSocket;

	clientSocket = accept(this->_socket, NULL, NULL);
	if (clientSocket == -1 || fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
		perror("client accepting sys calls failed");
	else
	{
		this->pfds.push_back((struct pollfd){clientSocket, POLLIN, 0});
		Client *newClient = new Client(clientSocket);
		this->_clients[clientSocket] = newClient;
	}
}

void Server::handleEstablishedClientEvents(void)
{
	std::vector<std::string> commands;
	std::string requestMessage;

	for (size_t i = 1; i < this->pfds.size(); i++)
	{
		if (this->pfds[i].revents & POLLIN)
		{
			int clientFd = this->pfds[i].fd; // to avoid the case of the client closing the connection while processing the request
			if (Server::readRequest(requestMessage, clientFd) == -1)
			{
				Server::closeConnection(clientFd);
				continue;
			}
			this->_clients[clientFd]->concatCmdBuffer(requestMessage);
			Server::log(requestMessage, clientFd, false);
			if (requestMessage.find("\r\n") != std::string::npos)
			{
				requestMessage = this->_clients[clientFd]->getCmdBuffer();
				commands = split(requestMessage, MESSAGE_DELIMITER);
				Server::executeCommands(commands, clientFd);
				if (this->_clients.find(clientFd) != this->_clients.end()) // this handles the case the client connection was closed because of a command
					this->_clients[clientFd]->clearCmdBuffer();
			}
		}
	}
}

int Server::readRequest(std::string &message, const int fd)
{
	char buf[Server::RECV_BUFFER_SIZE];

	int bytesReceived = recv(fd, buf, Server::RECV_BUFFER_SIZE, 0);
	if (bytesReceived == -1)
		perror("recv failed");
	else if (bytesReceived == 0)
	{
		log("connection closed", fd, true);
		return (-1);
	}
	else
	{
		buf[bytesReceived] = 0;
		message.append(buf);
		while (bytesReceived)
		{
			bytesReceived = recv(fd, buf, Server::RECV_BUFFER_SIZE, 0);
			if (bytesReceived == -1)
			{
				if (errno != EWOULDBLOCK)
					perror("recv failed");
				return (0);
			}
			buf[bytesReceived] = 0;
			message.append(buf);
			if (message.size() > Server::MAX_BUFFER_READ)
				break;
		}
	}
	return (0);
}

void Server::coreProcess(void)
{
	int numOfEventsOccured;

	std::cout << "Server started on port: " << this->_port << std::endl;
	while (1)
	{
		numOfEventsOccured = poll(this->pfds.data(), this->pfds.size(), -1);
		if (numOfEventsOccured == -1)
			perror("poll system-call failed");
		if ((this->pfds[Server::SERVER_SOCKET_INDEX].revents & POLLIN))
		{
			Server::handleNewClient();
			numOfEventsOccured--;
		}
		if (numOfEventsOccured >= 1)
			Server::handleEstablishedClientEvents();
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

std::string getCurrentTime()
{
	time_t rawTime;
	struct tm *timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	char buffer[80];
	strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", timeInfo);

	return buffer;
}

void Server::log(const std::string &message, int fd, bool send)
{
	if (send)
		std::cout << "\033[31m" << getCurrentTime() << "\033[0m"
				  << " "
				  << "\e[0;32m"
				  << "S " << fd << "\033[0m"
				  << " \"" << removeTrailingCRLF(message) << "\"" << std::endl;
	else
		std::cout << "\033[31m" << getCurrentTime() << "\033[0m"
				  << " "
				  << "\e[0;32m"
				  << "R " << fd << "\033[0m"
				  << " \"" << removeTrailingCRLF(message) << "\"" << std::endl;
}

void Server::sendReply(const std::string &message, int clientFd)
{
	log(message, clientFd, true);
	if (send(clientFd, message.c_str(), strlen(message.c_str()), 0) == -1)
		perror("send sys call failed: ");
}

int Server::setupServerSocket(void)
{
	int serverSocket;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));

	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("setsockopt failed");

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(this->getPort());

	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("bind failed");

	if (listen(serverSocket, SOMAXCONN) < 0)
		throw std::runtime_error("listen failed");
	return (serverSocket);
}
