#include "BMOBot.hpp"

const int BMOBot::RECV_BUFFER_SIZE = 1000;

BMOBot::BMOBot(const int port)
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == -1)
		throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	if (connect(this->_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("Socket connection failed: " + std::string(strerror(errno)));
	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == -1) // i think the order of set non-block mode does not affects
		throw std::runtime_error("Set Socket on Non-blocking mode failed: " + std::string(strerror(errno)));
	this->pfd.fd = this->_socket;
	this->pfd.events = POLLIN;
}

int BMOBot::getBotSocket(void)
{
	return (this->_socket);
}

int BMOBot::readBotRequest(std::string &message, const int fd)
{
	char buf[BMOBot::RECV_BUFFER_SIZE];

	int bytesReceived = recv(fd, buf, BMOBot::RECV_BUFFER_SIZE, 0);
	if (bytesReceived == -1)
		perror("recv failed");
	else if (bytesReceived == 0)
	{
		// log("connection closed", fd, true);
		std::cout << "connection closed" << std::endl;
		return (-1);
	}
	else
	{
		std::cout << "HERE" << std::endl;
		buf[bytesReceived] = 0;
		message.append(buf);
		while (bytesReceived)
		{
			bytesReceived = recv(fd, buf, BMOBot::RECV_BUFFER_SIZE, 0);
			if (bytesReceived == -1)
			{
				if (errno != EWOULDBLOCK)
					perror("recv failed");
				return (0);
			}
			buf[bytesReceived] = 0;
			message.append(buf);
			std::cout << "time" << std::endl;
		}
		std::cout << "finish" << std::endl;
	}
	return (0);
}

void BMOBot::botCoreProcess(void)
{
	while (1)
	{
		poll(&this->pfd, 1, -1);
		std::string msg;
		this->readBotRequest(msg, this->getBotSocket());
		std::cout << "msg read from server: " << msg << std::endl;
		// handle request
		// send result
	}
}