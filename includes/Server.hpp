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
	void parseCommands(const std::vector<std::string> commands, int clientFd);
	int  readRequest(std::string &message, const int fd);
	static void responseMsg(const std::string message, unsigned int clienFd);
	void handleNewClient(void);
	void handleEstablishedClientEvents(void);

	void closeConnection(int fd);

	// getters:
	int getPort(void);
	int getServerSocket(void);
	std::string getPassword(void);

	// setters:
	void setPort(const int port);
	void setServerSocket(const int serverSocket);
	void setPassword(const std::string password);

private:
	static const int recvBufferSize;

	int _port;
	int _socket;
	std::string _password;
	std::map<int, Client *> _clients;
	std::vector<struct pollfd> pfds;
};