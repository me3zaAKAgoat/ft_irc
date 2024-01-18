#include "Server.hpp"

int Server::port = 0;
std::vector<Client>	Server::x;
int Server::serverSocket = 0;
int Server::clientSocket = 0;
std::string Server::password = "";

void	cleanupResources(void)
{
    // Close the sockets when done
	// the order of closing affect smt or not?
    close(Server::getClientSocket());
    close(Server::getServerSocket());
}

std::string	joinStrs(std::vector<std::string>::iterator itBegin, std::vector<std::string>::iterator itEnd, std::string separator = "")
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

void	Server::addClient(Client &newClient)
{
		std::cout << "password is correct - client added" << std::endl;
		Server::x.push_back(newClient);
}

bool	Server::ReceiveRequest(std::string &message)
{
	char	msg[100];

	int bytesReceived = recv(Server::getClientSocket(), msg, sizeof(msg), 0);
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

void Server::parseCommands(const std::vector<std::string> commands, Client &newClient)
{
	std::vector<std::string> cmd;

	for (size_t i = 0; i < commands.size(); i++)
	{
		cmd = ft_split(commands[i], " ");
		if (cmd[0] == "PASS")
			PASS(cmd, newClient);
		else if (cmd[0] == "NICK")
			NICK(cmd, newClient);
		else if (cmd[0] == "USER")
			USER(cmd, newClient);
	}
}

bool isValidArgs(const int argc, const char *argv[])
{
	if (argc != 3)
		return (perror("args is not enough OR more than expected\n"), false);
	else if (!isValidPassword(argv[1]))
		return (werror("Error: invalid password\n"), false);
	else if (!isValidPort(argv[2]))
		return (werror("Error: invalid port\n"), false);
	return (true);
}

bool isValidPassword(const std::string password)
{
	if (password == "")
		return (false);
	return (true);
}

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

// what if there is multiple spaces mtab3ine
std::vector<std::string> ft_split(const std::string &input, const std::string &separator)
{
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t found = input.find(separator);
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

void setupSocket(void)
{
	// 1. Creating socket file descriptor
	Server::setServerSocket(socket(AF_INET, SOCK_STREAM, 0));
	if (Server::getServerSocket() < 0)
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
	if (setsockopt(Server::getServerSocket(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}

	// 3.0 struct sockaddr_in setup
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(Server::getPort());

	// 3.1 binding
	if (bind(Server::getServerSocket(), (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// 4. listening
	// SOMAXCONN : Socket Max Connection [128] (backlog)
	if (listen(Server::getServerSocket(), SOMAXCONN) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Waiting for a connection..." << std::endl;

	// 5.  accept
	// check if the accept method is fails or not 
	Server::setClientSocket(accept(Server::getServerSocket(), nullptr, nullptr));
	std::cout << "Connection established with a client" << std::endl;
}

void werror(const std::string msgError)
{
	std::cerr << msgError;
}

int Server::getPort(void)
{
	return (Server::port);
}

int Server::getServerSocket(void)
{
	return (Server::serverSocket);
}

int Server::getClientSocket(void)
{
	return (Server::clientSocket);
}

std::string Server::getPassword(void)
{
	return (Server::password);
}

void Server::setPort(const int port)
{
	Server::port = port;
}

void Server::setServerSocket(const int serverSocket)
{
	Server::serverSocket = serverSocket;
}

void Server::setClientSocket(const int clientSocket)
{
	Server::clientSocket = clientSocket;
}

void Server::setPassword(const std::string password)
{
	Server::password = password;
}


void	Server::responseMsg(const std::string message)
{
	char msg[100];

	size_t i = 0;
	while (i < message.size())
	{
		msg[i] = message[i];
		i++;
	}
	msg[i] = '\0';
	// std::cout << "the msg will be send is: "<< msg << std::endl;
	if (send(Server::getClientSocket(), msg, strlen(msg), 0) == -1)
		perror("send failed");
	else
		std::cout << "response sent" << std::endl;
}
