#pragma once

#include "Irc.hpp"

class Server
{
	public:
		Server(const int port, const std::string password);
		~Server();

		int							setupServerSocket(void);
		void						coreProcess(void);
		void						parseCommands(const std::vector<std::string> commands, int clientFd);
		int							readRequest(std::string &message, const int fd);
		static void					sendReply(const std::string &message, int clientFd);
		void						handleNewClient(void);
		void						handleEstablishedClientEvents(void);
		void						closeConnection(int fd);
		void						addChannel(Channel *channel);
		void						removeChannel(Channel *channel);

		std::map<int, Client *>		getClients(void);
		std::vector<Channel *>		getChannels(void);
		Channel*					getChannelByName(const std::string name);
		Client*						getClientByNickname(const std::string nickname);
		int							getPort(void);
		int							getServerSocket(void);
		std::string					getPassword(void);
		std::string 				getHostname(void);

		void						setPort(const int port);
		void						setServerSocket(const int serverSocket);
		void						setPassword(const std::string password);

		static void					log(const std::string& message, int fd, bool send);


	private:
		static const int			RECV_BUFFER_SIZE;
		static const int			SERVER_SOCKET_INDEX;

		int							_port;
		int							_socket;
		std::string					_hostname;
		std::string					_password;
		std::map<int, Client *>		_clients;
		std::vector<Channel *>		_channels;
		std::vector<struct pollfd>	pfds;
};