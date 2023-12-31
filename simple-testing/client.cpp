// client.cpp
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // Client-side socket code here...

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(8080);

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    std::cout << "Connected to the server" << std::endl;
    char message[10];
    while (1)
    {
        std::cout << std::endl << "type message to server..." << std::endl << std::endl;
        std::cin.getline(message, sizeof(message));
        message[9] = '\0';
        if (!strcmp(message, "exit"))
        {
            std::cout << "client exit" << std::endl;
            break;
        }
        if (send(clientSocket, message, sizeof(message), 0) == -1)
            perror("send failed");
    }
    // Close the socket when done
    close(clientSocket);

    return 0;
}