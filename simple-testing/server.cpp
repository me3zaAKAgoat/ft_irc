// server.cpp
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // 1. Creating socket file descriptor
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverSocket < 0)
    {
		perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Forcefully attaching socket to the port 8080

	// SOL_SOCKET flag:
	// is used for socket-level options.

	// SO_REUSEADDR flag:
	// This option allows the reuse of a local address (IP address and port number) 
	// even if it's still in a TIME_WAIT state after the socket is closed. 
	// This is useful when restarting a server to bind to the same address without waiting for the TIME_WAIT period to expire.
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
    	perror("setsockopt failed");
    	exit(EXIT_FAILURE);
    }

	// 3.0 struct sockaddr_in setup
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);


	// 3.1  binding
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
    	perror("bind failed");
    	exit(EXIT_FAILURE);
    }

	// 4.  listening
	// SOMAXCONN : Socket Max Connection [128]
    if (listen(serverSocket, 5) < 0) 
	{
    	perror("listen failed");
    	exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for a connection..." << std::endl;

	// 4.  accept
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    std::cout << "Connection established with a client" << std::endl;

	char message[100];
	while (1)
	{
		int bytesReceived = recv(clientSocket, message, sizeof(message), 0);
		message[bytesReceived] = '\0';
		std::cout << message << std::endl;
		if (bytesReceived == -1)
			perror("recv failed");
		else if (bytesReceived == 0)
		{
			perror("the remote side has closed the connection on you!");
			break;
		}
		
		if (!strcmp(message, "\0"))
			continue;
		else 
		{
			char response[100];
			strcpy(response, "Server: the message [");
        	strcat(response, message);
        	strcat(response, "] is recieved from client with success\n");
			if (send(clientSocket, response, strlen(response), 0) == -1)
            	perror("send failed");
		}
	}

    // Close the sockets when done
    close(clientSocket);
    close(serverSocket);

    return 0;
}
