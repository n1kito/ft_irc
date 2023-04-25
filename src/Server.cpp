#include "Server.hpp"

/* CONSTRUCTORS ***************************************************************/

#include <cerrno>
#include "numericReplies.hpp"

Server::Server() {}
Server::Server(int port, std::string password) : _port(port), _password(password), _creationDate(_getCurrentDate())
{
	PRINT("Time is", _creationDate);
	initCommands();
	std::cout << RPL_WELCOME("serveur", "nickname", "network") << std::endl;
	// 1) SERVER SOCKET
	// create ServerSocket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
		perror("Error creating socket");
        throw std::runtime_error("Error creating socket");
    }
	// configure setsockopt to make sure port number is reusable
	int opt =1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Erreur lors de la configuration de setsockopt");
        close(serverSocket);
	    throw std::runtime_error("Erreur lors de la configuration de setsockopt");
    }
	// configure address and port for server socket
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	
	// bind server socket
	int result = bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr));
    if (result < 0) {
        throw std::runtime_error("Error binding socket");        
    }

	// set server socket to listen
	if (listen(serverSocket, SOMAXCONN) == -1)
		throw std::runtime_error("Could not set server socket to listening state");

	// 2) EPOLL
	// create epoll instance
	int epollFd = epoll_create1(0);
	if (epollFd == -1) {
		throw std::runtime_error("Error creating epoll instance");        
	}
	// add server socket to epoll instance
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = serverSocket;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
		throw std::runtime_error("Error adding server socket to epoll instance");        
	}
	// in the loop, monitor events and if event on server socket, add new client to epoll,
	// else, handle client event
	while (true)
	{
		struct epoll_event events[MAX_EVENTS];
		int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
		if (numEvents == -1) {
			throw std::runtime_error("Error epoll_wait");        
		}
		std::cout << "numEvents: " << numEvents << "\n";
		for (int i = 0; i < numEvents; i++)
		{
			if (events[i].data.fd == serverSocket)
			{
				// accept the connection
				struct sockaddr_in clientAddress;
				socklen_t clientaddrlen = sizeof(clientAddress);
				// int client_socket = accept(serverSocket, NULL, 0);
				int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientaddrlen);	
				if (clientSocket == -1)
				{
					if ( errno != EAGAIN && errno != EWOULDBLOCK )
						throw std::runtime_error("Error connecting with client");
					continue ;
				}
				// add the new client socket to the epoll instance
				struct epoll_event event;
				event.events = EPOLLIN | EPOLLET;
				event.data.fd = clientSocket;
				if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1)
					throw std::runtime_error("Error adding client");
				addClient(clientSocket, Client(clientSocket));
				// Send RPL_WELCOME
				// TODO: change values
				send(clientSocket, RPL_WELCOME("pouet", "pouet", "pouet").c_str(), RPL_WELCOME("pouet", "pouet", "pouet").length(), 0);
			}
			else
			{
				// handle data from the client socket
				int clientSocket = events[i].data.fd;
				// read data from the client socket
				// process the data
				char buffer[1024];
      			int received = recv(clientSocket, buffer, sizeof(buffer), 0);

      			// Si la réception est inférieure ou égale à 0, le client s'est déconnecté.
      			if (received <= 0) {
        			std::cout << "Client disconnected" << std::endl;
        			close(clientSocket);
					// removeClient( clientSocket ); // remove from the client map and close fd
      			}
				else
				{
					// process the data
        			std::cout << "Received from client: " << "\"" << MAGENTA << buffer << RESET << "\"" << std::endl;
        			// send(clientSocket, handleRequest(_clients[clientSocket], buffer), response.length(), 0);
					handleRequest(_clients[clientSocket], buffer);
      			}
			}
		}
	}
	close(epollFd);
}



Server::Server(const Server &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Server::~Server()
{
	// std::cout << "Destructor called" << std::endl;
	// while(_commands.size() != 0)
	// {
	// 	delete _commands[0].first;

	// }
}

/* OVERLOADS ******************************************************************/

Server& Server::operator = (const Server &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* ACCESSORS ******************************************************************/

int									Server::getPort() const { return _port; }
std::string							Server::getPassword() const { return _password; }
std::map< int, Client >				Server::getClients() const { return _clients; }
const std::map< int, Client >*		Server::getClientsPtr() const { return &_clients; }
std::map< std::string, ACommand* >	Server::getCommands() const { return _commands; }
std::string							Server::getCreationDate() const { return _creationDate; }

void								Server::setPort( int port ) { _port = port; };
void								Server::setPassword( std::string password ) { _password = password; };
void								Server::setClients( std::map< int, Client > clients ) { _clients = clients; };
void								Server::setCommands( std::map< std::string, ACommand* > commands ) { _commands = commands; }

/* METHODS ********************************************************************/


void								Server::addClient( int fd, Client client )
{
	// _clients[fd] = client;
	_clients.insert( std::make_pair( fd, client ));
}

void								Server::removeClient( int fd )
{
	_clients.erase( fd );
	if( close( fd ) == -1 )
		throw std::runtime_error("Error when closing fd");
}

// This cannot work since numeric replies require specific arguments
// void								Server::sendNumericReplies(const Client& target, const int count, ...)
// {
// 	va_list	codesToSend;
// 	va_start(codesToSend, count);
// 	for (int i = 0; i < count; ++i)
// 	{
// 		std::string replyName = va_arg(codesToSend, char*);
// 		// TODO:
// 		// Get the correct message that corresponds to the name
// 		// Send it to the client
// 		(void)target;
// 	}
// 	va_end(codesToSend);
// }

// Add all command instances to the server's _commands map
void								Server::initCommands()
{
	_commands["NICK"] = new Nick(&_clients);
	_commands["USER"] = new User(&_clients);
}

void						Server::handleRequest(Client& client, const std::string& request)
{
	/*
		We handle the following commands
		-	CAP -> should not do anything
		- 	AUTHENTIFICATE <- not needed
		-	PASS
		-	NICK
		-	USER
		-	OPER
		-	QUIT
		-	ERROR
		Channel operations
		-	JOIN
		-	PART
		-	TOPIC
		-	NAMES
		-	LIST
		-	INVITE
		-	KICK
		... ?
	*/
	// Parse the request
	std::istringstream	requestStream(request);
	while(!requestStream.eof())
	{
		size_t		firstSpace;
		std::string	line;
		std::string	command;
		std::string	request;

		std::getline(requestStream, line);
		if (line.empty())
			break ;
		firstSpace = line.find(' ', 0);
		if (firstSpace == std::string::npos)
			break ;
		// PRINT("extracting command", "");
		command = line.substr(0, firstSpace);
		// PRINT("extracting request", "");
		request = line.substr(firstSpace, std::string::npos);
		// PRINT("line", line);
		PRINT("command", command);
		// PRINT("request", request);
		if (_commands.count(command) != 0)
		{
			std::cout << "Calling handleRequest() for " << command << std::endl;
			const std::string reply = _commands[command]->handleRequest(client, request); 
			send(client.getClientSocket(), reply.c_str(), reply.length(), 0);
		}
	}
}

// Returns a human readable string of the current date
std::string				Server::_getCurrentDate() const
{
	time_t		rawTime;
	struct tm*	timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);
	std::string	returnValue(asctime(timeInfo));
	return returnValue;
}


// ChatGPT explains epoll():

/*
	Sure, I can give you a brief rundown on how to use `epoll()` to monitor sockets in the context of a C++ program that acts as an IRC server.

	1. Create the epoll instance
	You can create an epoll instance using the `epoll_create()` function. This function returns an epoll file descriptor that you will use in subsequent calls to `epoll_ctl()` and `epoll_wait()`.

	```c++
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1) {
		// handle error
	}
	```

	2. Add the server socket to the epoll instance
	You should add the server socket to the epoll instance using the `epoll_ctl()` function with `EPOLL_CTL_ADD` as the operation. 

	```c++
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = server_socket;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &event) == -1) {
		// handle error
	}
	```

	3. Wait for events
	You can wait for events using the `epoll_wait()` function. This function blocks until there are events to process, or until a timeout occurs. When there are events to process, `epoll_wait()` returns an array of `struct epoll_event` that describes the events.

	```c++
	const int MAX_EVENTS = 10;
	struct epoll_event events[MAX_EVENTS];

	int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
	if (num_events == -1) {
		// handle error
	}
	```

	4. Process events
	You should loop through the array of `struct epoll_event` and handle each event. If the event is for the server socket, you should accept the connection and add the new client socket to the epoll instance.

	```c++
	for (int i = 0; i < num_events; i++) {
		if (events[i].data.fd == server_socket) {
			// accept the connection
			int client_socket = accept(server_socket, nullptr, nullptr);
			if (client_socket == -1) {
				// handle error
			}

			// add the new client socket to the epoll instance
			struct epoll_event event;
			event.events = EPOLLIN | EPOLLET;
			event.data.fd = client_socket;
			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event) == -1) {
				// handle error
			}
		} else {
			// handle data from the client socket
			int client_socket = events[i].data.fd;
			// read data from the client socket
			// process the data
		}
	}
	```

	5. Close the epoll instance
	Finally, when you are finished using the epoll instance, you should close it using the `close()` function.

	```c++
	close(epoll_fd);
	```

	Note that this is just a brief overview of how to use `epoll()` to monitor sockets in a C++ program that acts as an IRC server. There are many details that you will need to consider when implementing a production-quality IRC server.
*/