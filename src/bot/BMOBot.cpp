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
	this->nickname = "BMObot";
}

void sendBotReply(const std::string &message, int clientFd)
{
	if (send(clientFd, message.c_str(), strlen(message.c_str()), 0) == -1)
		perror("send sys call failed: ");
}

void	BMOBot::handleDateCmd(std::string& clientNickname)
{
	std::time_t currentTime = std::time(0);
	const std::tm *localTime = std::localtime(&currentTime);
	std::ostringstream oss;
	if (localTime != nullptr)
		oss << std::put_time(localTime, "%A%e %B %Y - %H;%M"); // search a bit about put_time
	else
		oss << "Failed to get the current date and time.";
	sendBotReply(RPL_PRIVMSG(this->nickname, clientNickname, oss.str()), this->getBotSocket());
}

void	BMOBot::handleUsageCmd(std::string& clientNickname)
{
	sendBotReply(RPL_PRIVMSG(this->nickname, clientNickname, "- list of available commands -"), this->getBotSocket());
	sendBotReply(RPL_PRIVMSG(this->nickname, clientNickname, this->getCmdsUsage()), this->getBotSocket());
}

void	BMOBot::invalidCmd(const commandData& cmd)
{
	std::string reply;
	reply = "'" + cmd.arguments[1] + "' is unavailable command, try USAGE command to see list of available commands";
	sendBotReply(RPL_PRIVMSG(this->nickname, cmd.prefix, reply), this->getBotSocket());
}

void BMOBot::botRegistration(const std::string &password) const
{
	std::string registration = "PASS " + std::string(password) + MESSAGE_DELIMITER + "NICK " + this->nickname + MESSAGE_DELIMITER + "USER x x x x" + MESSAGE_DELIMITER;
	sendBotReply(registration, this->_socket);
}

int BMOBot::getBotSocket(void) const
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

std::string BMOBot::getCmdsUsage(void)
{
	std::string cmdUsage;
	cmdUsage.append("DATE - description ~> Get the current date and time."); // idk why I can't add more cmds, newline (\n) doesn't works
	return (cmdUsage);
}

void BMOBot::greetAndProvideCommands(std::string clientNickname)
{
	std::string reply = "Hello, " + clientNickname + "! I'm BMOBot. Here is a list of available commands...";
	sendBotReply(RPL_PRIVMSG(this->nickname, clientNickname, reply), this->getBotSocket());
	sendBotReply(RPL_PRIVMSG(this->nickname, clientNickname, this->getCmdsUsage()), this->getBotSocket());
}

void	BMOBot::cleanseCommandData(commandData& cmd)
{
	if (!cmd.prefix.empty())
		cmd.prefix = cmd.prefix.substr(1);
	int	lastElementIndx = cmd.arguments.size() - 1;
	cmd.arguments[lastElementIndx] = cmd.arguments[lastElementIndx].substr(0, (cmd.arguments[lastElementIndx].size() - 2));
}

void	BMOBot::commandProcess(commandData& cmd)
{
	if (cmd.name == "CLIENT") // sent by server ex: CLIENT <nickname> means a new client so send welcome-bot msg
		this->greetAndProvideCommands(cmd.arguments[0]);
	else
	{
		this->cleanseCommandData(cmd);
		if (cmd.name == "PRIVMSG")
		{
			if (cmd.arguments[1] == "DATE")
				this->handleDateCmd(cmd.prefix);
			else if (cmd.arguments[1] == "USAGE")
				this->handleUsageCmd(cmd.prefix);
			else
				this->invalidCmd(cmd);
		}
	}
}

void BMOBot::botCoreProcess(void)
{
	while (1)
	{
		poll(&this->pfd, 1, -1);
		std::string msg;
		if (this->readBotRequest(msg, this->getBotSocket()) == -1)
			break;
		commandData cmd = parseCommandMessage(msg);
		this->commandProcess(cmd);
	}
}
