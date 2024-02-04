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

void BMOBot::botRegistration(const std::string &password)
{
	std::string registration = "PASS " + std::string(password) + MESSAGE_DELIMITER + "NICK " + this->nickname + MESSAGE_DELIMITER + "USER x x x x" + MESSAGE_DELIMITER;
	sendBotReply(registration, this->_socket);
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

std::string BMOBot::getCmdUsage(void)
{
	std::string cmdUsage;
	cmdUsage.append("DATE - description ~> Get the current date and time.");
	return (cmdUsage);
}

void BMOBot::greetAndProvideCommands(std::vector<std::string> words)
{
	std::string reply = "Hello, " + words[1] + "! I'm BMOBot. Here is a list of available commands...";
	sendBotReply(RPL_PRIVMSG(this->nickname, words[1], reply), this->getBotSocket());
	sendBotReply(RPL_PRIVMSG(this->nickname, words[1], this->getCmdUsage()), this->getBotSocket());
}

void BMOBot::botCoreProcess(void)
{
	while (1)
	{
		poll(&this->pfd, 1, -1);
		std::string msg;
		if (this->readBotRequest(msg, this->getBotSocket()) == -1)
			break;
		std::cout << "msg read from server:\n"
				  << msg << std::endl
				  << std::endl;
		std::vector<std::string> words = split(msg, " ");
		std::string reply;
		if (words[0] == "CLIENT")
			this->greetAndProvideCommands(words);
		else
		{
			words[0] = words[0].substr(1);						// remove (:) nickname of client
			words[3] = words[3].substr(1);						// remove (:) first word in the msg
			words[3] = words[3].substr(0, words[3].size() - 2); // remove CRLF
			if (words[1] == "PRIVMSG")
			{
				if (words[3] == "DATE")
				{
					std::time_t currentTime = std::time(0);
					const std::tm* localTime = std::localtime(&currentTime);					
					std::ostringstream oss;
					if (localTime != nullptr)
						oss << std::put_time(localTime, "%A%e %B %Y - %H;%M"); // search a bit about put_time
					else
    					oss << "Failed to get the current date and time.";
					sendBotReply(RPL_PRIVMSG(this->nickname, words[0], oss.str()), this->getBotSocket());
				}
				else if (words[3] == "USAGE")
				{
					sendBotReply(RPL_PRIVMSG(this->nickname, words[1], "- list of available commands -"), this->getBotSocket());
					sendBotReply(RPL_PRIVMSG(this->nickname, words[0], this->getCmdUsage()), this->getBotSocket());
				}
				else
				{
					reply = "'" + words[3] + "' is unavailable command, try USAGE command to see list of available commands";
					sendBotReply(RPL_PRIVMSG(this->nickname, words[0], reply), this->getBotSocket());
				}
			}
		}
		std::cout << "======================" << std::endl;
	}
}