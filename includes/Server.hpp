#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"

// utils-methods:
bool						isValidArgs(const int argc, const char *argv[]);
bool						isValidPort(const std::string port);
bool						isValidPassword(const std::string password);
std::vector<std::string>			ft_split(const std::string& input, const std::string& separator);
std::string						joinStrs(std::vector<std::string>::iterator itBegin, std::vector<std::string>::iterator itEnd, std::string separator);
void						werror(const std::string msgError);

void						setupSocket(void);

void						cleanupResources(void);

class Server
{
	public:
		// Server();
		// Server(const server& copy);
		// Server& operator=(const server& rhs);
		// ~Server();

		static void parseCommands(const std::vector<std::string> commands, Client &newClient);
		static bool	ReceiveRequest(std::string &message);
		static void	responseMsg(const std::string message);
		static void	addClient(Client &newClient);

		// getters:
		static int			getPort(void);
		static int			getServerSocket(void);
		static int			getClientSocket(void);
		static std::string		getPassword(void);

		// setters:
		static void			setPort(const int port);
		static void			setServerSocket(const int serverSocket);
		static void			setClientSocket(const int clientSocket);
		static void			setPassword(const std::string password);



	private:
		static int					port;
		static int					serverSocket;
		static int					clientSocket;
		static std::string				password;
		static std::vector<Client>	x;

};

#endif