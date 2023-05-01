#include "Server.hpp"

/* CONSTRUCTORS ***************************************************************/

#include <cerrno>
#include "numericReplies.hpp"


Server::Server() {}
Server::Server(const int& port, const std::string& password, const std::string& serverName) :
	_port(port),
	_password(password),
	_creationDate(_getCurrentDate()),
	_serverName(serverName)
{
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
				addClient(clientSocket, Client(clientSocket, _serverName));
				// Send RPL_WELCOME
				// TODO: change values
				// std::string nickname = _clients[clientSocket].getNickname();
				// send(clientSocket, RPL_WELCOME("server", "nickname", "network").c_str(), RPL_WELCOME("server", "nickname", "network").length(), 0);
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
					removeClient( clientSocket ); // remove from the client map and close fd
      			}
				else
				{
					// process the data
        			std::cout	<< std::endl
								<< "************ Received from client **********" << std::endl
								<< BOLD << "[" << RESET << DIM << "Request" << RESET << BOLD << "]" << RESET << std::endl
								<< MAGENTA << buffer << RESET << std::endl;
								std::cout << BOLD << "[" RESET << DIM << "Handling" << RESET << BOLD << "]" << RESET << std::endl;
					handleRequest(_clients[clientSocket], cleanBuffer(buffer));
					// if the client is authentificated (PASS NICK USER) and not RPL_WELCOMEd
					if (_clients[clientSocket].isAuthentificated() && _clients[clientSocket].getWelcomeState() == 0)
					{
						send(clientSocket, RPL_WELCOME("server",_clients[clientSocket].getNickname(), "network").c_str(), RPL_WELCOME("server", _clients[clientSocket].getNickname(), "network").length(), 0);
						_clients[clientSocket].setWelcomeState(true);
					}
					// else if authentification failed and not RPL_WELCOME, kill is sent to disconnect client
					else if (!_clients[clientSocket].isAuthentificated() && _clients[clientSocket].getWelcomeState() == 0 && !_clients[clientSocket].getPassword().empty())
					{
						std::cout << "NOT AUTHENTIFICATED AND NOT WELCOMED\n";
						//std::string msg = KILL(_clients[clientSocket].getNickname(), "authentification failed");                   
						//send(clientSocket, msg.c_str(), msg.length(), 0);
						removeClient(clientSocket);
					}

					std::cout	<< "********************************************"
								<< std::endl;
					// send(clientSocket, handleRequest(_clients[clientSocket], buffer), response.length(), 0);
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

Client& Server::operator[](const int fd) {
		return _clients[fd];
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
	std::cout << "ADDING CLIENT :" << fd << std::endl;
	_clients[fd] = client;
	// _clients.insert( std::make_pair( fd, client ));
	std::cout << "TOTAL CLIENTS :" << _clients.size() << std::endl;
}

void								Server::removeClient( int fd )
{
	// used to give the client enough time to print messages before closing the connection
	// usleep(1000); //TODO: there has to be a better way to do this
	// send a quit message to IRSSI
	// std::string message = "QUIT :Goodbye\r\n";
	// if (send(fd, message.c_str(), message.size(), 0) == -1) {
        // std::cerr << "Failed to send quit message to client socket " << fd << std::endl;
    // }
	std::cout << "\n[removeClient]\n _client.size:" << _clients.size() << "\n"; 
	if( close( fd ) == -1 )
		throw std::runtime_error("Error when closing fd");
	std::cout << RED_BLOC << "Map size before erasing: " << RESET << _clients.size() << std::endl;
	_clients.erase( fd );
	std::cout << RED_BLOC << "Map size after erasing: " << RESET << _clients.size() << std::endl;
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
	_commands["PING"] = new Ping(&_clients);
	_commands["PASS"] = new Pass(&_clients, _password);
}

void								Server::handleRequest(Client& client, const std::string& request)
{
	// static bool	firstRequest = true;
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
	// std::cout << YELLOW << "commands to run : " << RESET << requestStream.str() << std::endl;
	while(!requestStream.eof())
	{
		size_t		firstSpace;
		std::string	line;
		std::string	command;
		std::string	request;

		std::getline(requestStream, line);
		std::cout << line << "\n";
		if (line.empty())
			continue ;
		firstSpace = line.find(' ', 0);
		if (firstSpace == std::string::npos)
			break ;
		// PRINT("extracting command", "");
		command = line.substr(0, firstSpace);
		request = line.substr(firstSpace + 1, std::string::npos);
		PRINT("command", command);
		PRINT("request", request);
		if (_commands.count(command) != 0)
		{
			const std::string reply = _commands[command]->handleRequest(client, request); 
			// TODO
			send(client.getClientSocket(), reply.c_str(), reply.length(), 0);
			std::cout << "actual command: <" << command << ">" << std::endl;
			if (command == "PASS" && client.getPassword().empty())
			{
				std::cout << "ENTERING WRONG PASSWORD" << std::endl;
				usleep(1000); //TODO: there has to be a better way to do this
				//removeClient(client.getClientSocket());
				break ;
			}
		}
		}
}

// This function removes \r characters from the buffer.
std::string						Server::cleanBuffer(std::string buffer) const
{
	while (true)
	{
		size_t pos = buffer.find('\r', 0);
		if (pos == std::string::npos)
			break;
		buffer.erase(pos, 1);
	}
	return buffer;
}

// Returns a human readable string of the current date
std::string						Server::_getCurrentDate() const
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