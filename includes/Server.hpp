#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"

// utils-methods:
bool isValidArgs(const int argc, const char *argv[]);
bool isValidPort(const std::string port);
bool isValidPassword(const std::string password);
std::vector<std::string> ft_split(const std::string &input, const std::string &separator);
std::string joinStrs(std::vector<std::string>::iterator itBegin, std::vector<std::string>::iterator itEnd, std::string separator);
void werror(const std::string msgError);

class Server
{
	public:
		// Server();
		// Server(const server& copy);
		// Server& operator=(const server& rhs);
		// ~Server();
		static void setupServerSocket(void);
		static void	process(void);
		static void parseCommands(const std::vector<std::string> commands, unsigned int clientIndex);
		static bool ReceiveRequest(std::string &message, const int fd);
		static void responseMsg(const std::string message, unsigned int fdClient);
		static int	isEventInServerOrClientsFDs(unsigned int pollRet);
		static void	acceptNewConnection(void);
		static void	detectEventInClientsFds(void);
		static void addClient(Client &newClient);
		static void cleanupResources(void);

		// getters:
		static int getPort(void);
		static int getServerSocket(void);
		static std::string getPassword(void);

		// setters:
		static void setPort(const int port);
		static void setServerSocket(const int serverSocket);
		static void setPassword(const std::string password);

		// manage fds pollfd
		static void		pushBackFds(const int fd);
		static void		removeFdClient(const int fd);

	private:
		static int					port;
		static int					serverSocket;
		static int					clientSocket;
		static std::string			password;
		static std::vector<Client>	x;
		static struct pollfd		*fds;
		static size_t				size_fds;
};

#endif