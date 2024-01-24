#pragma once

#include "Irc.hpp"

// utils-methods:

std::vector<std::string> split(const std::string &input, const std::string &separator);
std::string joinStrs(std::vector<std::string>::iterator itBegin, std::vector<std::string>::iterator itEnd, std::string separator);

class Server
{
	public:
		Server();
		Server(const int port, const std::string password);
		// Server(const Server& copy);
		// Server& operator=(const Server& rhs);
		~Server();

		int	setupServerSocket(void);
		void coreProcess(void);
		void parseCommands(const std::vector<std::string> commands, unsigned int clientIndex);
		bool ReceiveRequest(std::string &message, const int fd);
		static	void responseMsg(const std::string message, unsigned int fdClient);
		void	handleNewClient(void);
		void	detectEventInClientsFds(void);
		void	addClient(Client &newClient);

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
		int						_port;
		unsigned int			_socket;
		std::string				_password;
		std::vector<Client>		_clients;
		struct pollfd			*fds;
		size_t					sizeFds;
};