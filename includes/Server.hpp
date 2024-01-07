#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"

// utils-methods:
bool						isValidArgs(const int argc, const char *argv[]);
bool						isValidPort(const string port);
bool						isValidPassword(const std::string password);
std::vector<string>			split(const string& input, const string& separator);
string						joinStrs(std::vector<string>::iterator itBegin, std::vector<string>::iterator itEnd, string separator);
void						werror(const string msgError);

void						setupSocket(void);

void						cleanupResources(void);

class Server
{
	public:
		// Server();
		// Server(const server& copy);
		// Server& operator=(const server& rhs);
		// ~Server();

		static void parseCommands(const std::vector<string> commands, Client &newClient);
		static bool	ReceiveRequest(string &message);
		static void	responseMsg(const string message);
		static void	addClient(Client &newClient);

		// getters:
		static int			getPort(void);
		static int			getServerSocket(void);
		static int			getClientSocket(void);
		static string		getPassword(void);

		// setters:
		static void			setPort(const int port);
		static void			setServerSocket(const int serverSocket);
		static void			setClientSocket(const int clientSocket);
		static void			setPassword(const string password);



	private:
		static int					port;
		static int					serverSocket;
		static int					clientSocket;
		static string				password;
		static std::vector<Client>	x;

};

#endif