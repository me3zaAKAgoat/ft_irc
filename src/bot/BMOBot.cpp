#include "BMOBot.hpp"

const int BMOBot::RECV_BUFFER_SIZE = 1000;

BMOBot::BMOBot(const int port, std::string serverIp)
{
	struct addrinfo hints, *res;

	// first, load up address structs with getaddrinfo():

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	// this part is to convert the port from int to string C98 style
	std::stringstream ss;
	ss << port;
	// getaddrinfo gets server data that will be used by the bot socket to connect to the server
	if (getaddrinfo(serverIp.c_str(), ss.str().c_str(), &hints, &res) != 0)
		throw std::runtime_error("getaddrinfo failed: " + std::string(strerror(errno)));
	
	this->_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (this->_socket < 0)
		throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));
	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Set Socket on Non-blocking mode failed: " + std::string(strerror(errno)));
	if (connect(this->_socket, res->ai_addr, res->ai_addrlen) < 0 && errno != EINPROGRESS)
		throw std::runtime_error("Socket connection failed: " + std::string(strerror(errno)));

	// poll to wait/check for the socket to be ready to write
	struct pollfd pfd;
	pfd.fd = this->_socket;
	pfd.events = POLLOUT;
	int writeable = poll(&pfd, 1, 1000);
	if ((pfd.revents & (POLLERR | POLLHUP | POLLNVAL) ) || !writeable) // check for errors
		throw std::runtime_error("Socket connection failed.");

	std::cout << "Connected to server" << std::endl;

	this->pfd.fd = this->_socket;
	this->pfd.events = POLLIN;
	this->nickname = "BMOBot";
	this->username = "BMOBot";
	this->serverIp = serverIp;
}

BMOBot::~BMOBot(void)
{
	close(this->_socket);
}

void sendBotReply(const std::string &message, int clientFd)
{
	if (send(clientFd, message.c_str(), strlen(message.c_str()), 0) < 0)
		perror("send sys call failed: ");
}

void BMOBot::handleDateCmd(std::string &clientNickname)
{
	time_t rawTime;
	struct tm* timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	char buffer[80];
	strftime(buffer, sizeof(buffer), "[%Y-%m-%d %Hh%Mm%S]", timeInfo);

	std::ostringstream oss;
	oss << buffer;
	sendBotReply(RPL_NOTICE(formulatePrefix(this->serverIp, this->nickname, this->username), clientNickname, oss.str()), this->getBotSocket());
}

void BMOBot::handleUsageCmd(std::string &clientNickname)
{
	sendBotReply(RPL_NOTICE(formulatePrefix(this->serverIp, this->nickname, this->username), clientNickname, "- list of available commands -"), this->getBotSocket());
	sendBotReply(RPL_NOTICE(formulatePrefix(this->serverIp, this->nickname, this->username), clientNickname, this->getCmdsUsage()), this->getBotSocket());
}

void BMOBot::botRegistration(const std::string &password) const
{
	std::string registrationCmd = "PASS " + password + MESSAGE_DELIMITER + "NICK " + this->nickname + MESSAGE_DELIMITER + "USER " + this->username + " 0 * " + this->username + MESSAGE_DELIMITER;
	sendBotReply(registrationCmd, this->_socket);
}

int BMOBot::getBotSocket(void) const
{
	return (this->_socket);
}

int BMOBot::readBotRequest(std::string &message, const int fd)
{
	char buf[BMOBot::RECV_BUFFER_SIZE];

	int bytesReceived = recv(fd, buf, BMOBot::RECV_BUFFER_SIZE, 0);
	if (bytesReceived < 0)
		perror("recv failed");
	else if (bytesReceived == 0)
	{
		std::cout << "connection closed" << std::endl;
		return (-1);
	}
	else
	{
		buf[bytesReceived] = 0;
		message.append(buf);
		while (bytesReceived)
		{
			bytesReceived = recv(fd, buf, BMOBot::RECV_BUFFER_SIZE, 0);
			if (bytesReceived < 0)
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

void BMOBot::invalidCmd(const std::string& commandName, const std::string& clientNickname)
{
	std::string reply;
	reply = "'" + commandName + "' is unavailable command, try USAGE command for a list of available commands";
	sendBotReply(RPL_NOTICE(formulatePrefix(this->serverIp, this->nickname, this->username), clientNickname, reply), this->getBotSocket());
}

std::string BMOBot::getCmdsUsage(void)
{
	std::string cmdUsage("DATE - description ~> Get the current date and time.");
	return (cmdUsage);
}

void BMOBot::greetAndProvideCommands(std::string clientNickname)
{
	std::string reply = "Hello, " + clientNickname + "! I'm BMOBot. Here is a list of available commands...";
	sendBotReply(RPL_NOTICE(formulatePrefix(this->serverIp, this->nickname, this->username), clientNickname, reply), this->getBotSocket());
	sendBotReply(RPL_NOTICE(formulatePrefix(this->serverIp, this->nickname, this->username), clientNickname, this->getCmdsUsage()), this->getBotSocket());
}

void BMOBot::processCommand(commandData &cmd)
{
	if (cmd.name == "CLIENT") // sent by server ex: CLIENT <nickname> means a new client so send welcome-bot msg
		this->greetAndProvideCommands(cmd.arguments[0]);
	else if (cmd.name == "PRIVMSG")
	{
		std::string clientNick = cmd.prefix.substr(1, cmd.prefix.find('!') - 1);
		std::string commandName = removeTrailingCRLF(cmd.arguments[1]);
		if (commandName == "DATE")
			this->handleDateCmd(clientNick);
		else if (commandName == "USAGE")
			this->handleUsageCmd(clientNick);
		else
			this->invalidCmd(commandName, clientNick);
	}
}

void BMOBot::botCoreProcess(void)
{
	while (1)
	{
		if (poll(&this->pfd, 1, -1) < 0)
			perror("poll system-call failed");
		std::string msg;
		if (this->readBotRequest(msg, this->getBotSocket()) < 0)
			break;
		commandData cmd = parseCommandMessage(msg);
		this->processCommand(cmd);
	}
}
