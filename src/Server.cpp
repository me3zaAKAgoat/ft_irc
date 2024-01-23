#include "Server.hpp"

int					Server::port = 0;
std::vector<Client>	Server::x;
int					Server::serverSocket = -1;
int					Server::clientSocket = -1;
std::string			Server::password = "";
size_t				Server::size_fds = 0;
struct pollfd		*Server::fds = NULL; // struct pollfd		Server::*fds = NULL;

void Server::cleanupResources(void)
{
	// Close the sockets when done
	// the order of closing affect smt or not?
	// close(Server::getClientSocket()); // close all fd-socket-clients
	close(Server::getServerSocket());
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
	Server::x.push_back(newClient);
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

void handleRegistrationCommand(const std::string commands, Client &client)
{
	std::vector<std::string> cmdSplit;

	cmdSplit = ft_split(commands, " ");
	if (cmdSplit[0] == "PASS")
		PASS(commands, client, cmdSplit.size());
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
	std::vector<Client>::iterator client = Server::x.begin() + clientIndex;
	for (size_t i = 0; i < commands.size(); i++)
	{
		cmd = ft_split(commands[i], " ");
		if (isRegistrationCommand(cmd[0]))
			handleRegistrationCommand(commands[i], (*client));
		else
			std::cout << "invalid command" << std::endl;
	}
	std::cout << "msg from client-" << clientIndex + 1 << "-" << Server::x[clientIndex].getNickName() << std::endl;
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
		return (std::cout << "Error: password-argument empty!" << std::endl, false);
	return (true);
}

bool isValidPort(const std::string port)
{
	int i = 0;
	if (port == "")
		return (std::cout << "Error: port-argument empty!" << std::endl, false);
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
	if (found == std::string::npos)
	{
		if (separator != " ")
			std::cout << "separator doesn't found" << std::endl;
		result.push_back(input);
		return (result);
	}
	while (found != std::string::npos)
	{
		if (input.substr(start, found - start) != "")
			result.push_back(input.substr(start, found - start));
			start = found + separator.size(); // Move past the separator
		found = input.find(separator, start);
	}
	if (input.substr(start) != "")
		result.push_back(input.substr(start));
	return (result);
}

// Return Value: 
//		(0) –> event occurs only at the fd-server
//		(1) –> event occurs at the fd-server and other fd-client(s)
//		(2) –> event occurs only at fd-client(s)
//		(-1) –> Error (if all is normal shouldn't be returned)
int	Server::isEventInServerOrClientsFDs(unsigned int pollRet)
{
	if (pollRet == 1 && Server::fds[0].revents)
		return (0);
	else if (pollRet > 1 && Server::fds[0].revents & POLLIN)
		return (1);
	else if (pollRet >= 1 && !(Server::fds[0].revents & POLLIN))
		return (2);
	std::cout << "return poll: " << pollRet << std::endl;
	std::cout << "Server::fds[0].revents: " << Server::fds[0].revents << std::endl;
	return (-1);
}

void	Server::acceptNewConnection(void)
{
	Client	newClient;
	int		acceptRet;

	acceptRet = accept(Server::getServerSocket(), nullptr, nullptr);
	if (acceptRet == -1)
		perror("accept system-call failed"); // try to check the errno
	else
	{
		Server::pushBackFds(acceptRet);
		newClient.setFd(acceptRet);
		Server::x.push_back(newClient);
		std::cout << "Connection established with a client: " << acceptRet - 3 << std::endl;
		Server::responseMsg("Start...\n", newClient.getFd());
		Server::responseMsg("––> type a message: ", newClient.getFd());
	}
}


void	Server::detectEventInClientsFds(void)
{
POLLERR;
POLLHUP;
POLLIN;
POLLNVAL;
POLLOUT;
POLLPRI;
POLLRDBAND;
POLLRDNORM;
POLLWRBAND;
POLLWRNORM;
	std::vector<std::string>	commands;

	for (size_t i = 1; i < Server::size_fds; i++)
	{
		if (Server::fds[i].revents & POLLIN)
		{
			std::string msg;
			if (!Server::ReceiveRequest(msg, fds[i].fd))
			{
				// if (Server::fds[i].fd != Server::getServerSocket())
				// {
					Server::x.pop_back();
					Server::removeFdClient(Server::fds[i].fd);
					continue;
				// }
			}
			std::cout << "========== NEW MSG ========== : " << msg << std::endl;
			commands = ft_split(msg, "\n"); // add \r if using lime chat
			Server::parseCommands(commands, i - 1);
			std::cout << std::endl << "========== ======= ==========" << std::endl;
			Server::responseMsg("––> type a message: ", Server::fds[i].fd);
		}
		// else
		// 	std::cout << "event does not occurs at client: " << Server::fds[i].fd << std::endl;
	}
}

void	Server::process(void)
{
	int	pollRet;
	int	EventOccurence;

	while (1)
	{
		Server::fds[0].revents = 0;
		pollRet = poll(Server::fds, Server::size_fds, -1);
		if (pollRet == -1)
			perror("poll system-call failed");
		else
		{
			EventOccurence = Server::isEventInServerOrClientsFDs(pollRet);
			if (EventOccurence == 0 && (Server::fds[0].revents & POLLIN))
			{
				std::cout << "new connection" << std::endl;
				Server::acceptNewConnection();
			}
			else if (EventOccurence == 1)
			{
				std::cout << "new connection and new msg(s)" << std::endl;
				Server::acceptNewConnection();
				Server::detectEventInClientsFds();
			}
			else if (EventOccurence == 2)
				Server::detectEventInClientsFds();
		}
	}
}

void Server::setupServerSocket(void)
{
	int socketRet;

	// 1. Creating socket file descriptor
	socketRet = socket(AF_INET, SOCK_STREAM, 0);
	if (socketRet < 0)
		return (perror("socket failed"), exit(EXIT_FAILURE));
	Server::setServerSocket(socketRet);
	Server::pushBackFds(Server::getServerSocket());
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

void Server::setPassword(const std::string password)
{
	Server::password = password;
}

void Server::responseMsg(const std::string message, unsigned int fdClient)
{
	char msg[100];

	size_t i = 0;
	while (i < message.size())
	{
		msg[i] = message[i];
		i++;
	}
	msg[i] = '\0';
	if (send(fdClient, msg, strlen(msg), 0) == -1)
		perror("send failed");
}

void Server::pushBackFds(const int fd)
{
	struct pollfd *newFds;

	newFds = new struct pollfd[Server::size_fds + 1];
	size_t i = 0;
	while (i < Server::size_fds)
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
	if (Server::size_fds > 0)
		delete[] (Server::fds);
	Server::size_fds++;
	Server::fds = newFds;
}

void	Server::removeFdClient(const int fd)
{
	struct pollfd *newFds;

	newFds = new struct pollfd[Server::size_fds - 1];
	size_t i = 0;
	size_t j = 0;
	while (i < Server::size_fds)
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
	Server::size_fds--;
	Server::fds = newFds;
}