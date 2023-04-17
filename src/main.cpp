#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

#define IPURPLE		"\033[3;35m"
#define END_C		"\033[0;39m"

/*
Classes:
	- server
	- client
	- command
	- channel
*/

int	main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Wrong argument count" << std::endl;
		return 1;
	}
	int port = atoi(argv[2]);
	// Create a socket
	// Bind it to IP and port
	// Put it in listening state
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
		perror("Error creating socket");
        return 1;
    }

	int opt =1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Erreur lors de la configuration de setsockopt");
        close(serverSocket);
        return -1;
    }


    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    // bind the socket to a local address and port
    int result = bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr));
    if (result < 0) {
		perror("Error binding socket");
        return 1;
    }

    // set the socket to listen for incoming connections
    result = listen(serverSocket, 5);
    if (result < 0) {
		perror("Error listening on socket");
        // std::cerr << "Error listening on socket\n";
        return 1;
    }

	std::cout << "Server listening on port " << port << std::endl;

	// int clientSocket = accept(serverSocket, NULL, NULL);
	
	socklen_t addr_len = sizeof(addr);
	int clientSocket = accept(serverSocket, (struct sockaddr*)&addr, (socklen_t*)&addr_len);
    if (clientSocket == -1) {
        perror("Failed to accept incoming connection");
        return 1;
    }
	else // just for setup, added by mj
	{
		char ip_address[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(addr.sin_addr), ip_address, INET_ADDRSTRLEN);
		int port = ntohs(addr.sin_port);

		std::cout << "Accepted connection from " << ip_address << ":" << port << std::endl;
	}

	std::stringstream welcome_msg;
	welcome_msg	<< ":" << "server_name" << " 001 " << "mjallada"
				<< " :Welcome ! " << "mjallada" << "@" << "localhost" << "\r\n";
	// welcome_msg << ":server_name 001 <nick> :Welcome to the Internet Relay Network <nick>!<user>@<host>\r\n";
	std::string message = welcome_msg.str();
	send(clientSocket, message.c_str(), message.length(), 0);

    while (true) {
		char buffer[1024];
        int received = recv(clientSocket, buffer, sizeof(buffer), 0);

        // Si aucune donnée n'est reçue, fermez la connexion et sortez de la boucle
        if (received <= 0) {
			std::cout << "Did not receive anything" << std::endl;
            close(clientSocket);
            break;
        }
		std::cout << "Command sent ==> " << IPURPLE << buffer << END_C << std::endl;

    }
	std::cout << "left the loop" << std::endl;
    // accept incoming connections...
    
    result = close(serverSocket);
    if (result < 0) {
        std::cerr << "Error closing socket\n";
        return 1;
    }

	(void)argv;
	return 0;
}