#pragma once

#include "Irc.hpp"

class Server
{
public:
	Server();
	Server(const int port, const std::string password);
	// Server(const Server& copy);
	// Server& operator=(const Server& rhs);
	~Server();

	int setupServerSocket(void);
	void coreProcess(void);
	void parseCommands(const std::vector<std::string> commands, unsigned int clientIndex);
	int receiveRequest(std::string &message, const int fd);
	static void responseMsg(const std::string message, unsigned int fdClient);
	void handleNewClient(void);
	void handleEstablishedClientEvents(void);

	void closeConnection(Client &client);

	// getters:
	int getPort(void);
	int getServerSocket(void);
	std::string getPassword(void);

	// setters:
	void setPort(const int port);
	void setServerSocket(const int serverSocket);
	void setPassword(const std::string password);

	// manage fds pollfd
	void pushBackFds(const int fd);
	void removeFdClient(const int fd);

private:
	int _port;
	int _socket;
	std::string _password;
	std::vector<Client> _clients;
	static const int recvBufferSize;

	/* these two will be removed in favour of a vector, when passing to poll just call fds.data() and fds.size()*/
	std::vector<struct pollfd> pfds;
};