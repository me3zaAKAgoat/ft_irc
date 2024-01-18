// server.cpp
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>

bool isValidPort(const std::string port)
{
	int i = 0;
	while (port[i])
	{
		if (!isdigit(port[i]))
			return (perror("port should be a number\n"), false);
		i++;
	}
	return (true);
}

std::vector<std::string> ft_split(const std::string &input, const std::string &separator)
{
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t found = input.find(separator);
	while (found != std::string::npos)
	{
		result.push_back(input.substr(start, found - start));
		start = found + 2; // Move past the separator
		found = input.find(separator, start);
	}
	result.push_back(input.substr(start));
	return (result);
}

void setupSocket(int &serverSocket, int &clientSocket, const std::string port)
{
	// 1. Creating socket file descriptor
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// 2. Forcefully attaching socket to the port 8080

	// SOL_SOCKET flag:
	// is used for socket-level options.

	// SO_REUSEADDR flag:
	// This option allows the reuse of a local address (IP address and port number)
	// even if it's still in a TIME_WAIT state after the socket is closed.
	// This is useful when restarting a server to bind to the same address without waiting for the TIME_WAIT period to expire.
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}

	// 3.0 struct sockaddr_in setup
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(stod(port));

	// 3.1 binding
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// 4. listening
	// SOMAXCONN : Socket Max Connection [128]
	if (listen(serverSocket, SOMAXCONN) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Waiting for a connection..." << std::endl;

	// 5.  accept
	clientSocket = accept(serverSocket, nullptr, nullptr);
	std::cout << "Connection established with a client" << std::endl;
}

void parseCommand(const std::vector<std::string> cmd)
{
	if (cmd[0] == "PASS")
	{
		if (cmd[1] == "pass")
			std::cout << "SERVER: password is correct\r\n";
		else
			std::cout << "SERVER: password is incorrect\r\n";
	}
	else if (cmd[0] == "NICK")
	{
	}
	else if (cmd[0] == "USER")
	{
	}
	else
		std::cout << cmd[0] << " :Unknown command\r\n";
}

int main(int argc, const char *argv[])
{
	int serverSocket;
	int clientSocket;

	if (argc != 3)
		return (perror("args is not enough OR more than expected\n"), EXIT_FAILURE);

	std::string password = argv[1];
	std::string port = argv[2];
	if (!isValidPort(port))
		return (EXIT_FAILURE);
	setupSocket(serverSocket, clientSocket, port);

	char msg[100];
	std::string message;
	while (1)
	{
		int bytesReceived = recv(clientSocket, msg, sizeof(msg), 0);
		if (bytesReceived == -1)
			perror("recv failed");
		else if (bytesReceived == 0)
		{
			perror("the remote side has closed the connection on you!");
			break;
		}
		message = msg;
		std::cout << "========== NEW MSG ==========" << std::endl;
		std::cout << message << std::endl;
		// PASS hello\r\nNICK elmehdi\r\n
		std::vector<std::string> commands = ft_split(message, "\r\n");
		std::vector<std::string> cmd;
		size_t i = 0;
		while (i < commands.size())
		{
			cmd = ft_split(commands[i++], " ");
			parseCommand(cmd);
		}
	}

	// Close the sockets when done
	close(clientSocket);
	close(serverSocket);

	return (EXIT_SUCCESS);
}
