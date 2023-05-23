#include "Server.hpp"
/* CONSTRUCTORS ***************************************************************/

#include <cerrno>
#include "numericReplies.hpp"
#include "ft_irc.hpp"

Server::Server() {}
// Server::Server(const int& port, const std::string& password, const std::string& serverName) :
// 	_port(port),
// 	_password(password),
// 	_creationDate(getCurrentDate()),
// 	_serverName(serverName)

void		Server::launch(const int& port, const std::string& password, const std::string& serverName)
{
	_port = port;
	_password = password;
	_creationDate = getCurrentDate();
	_serverName = serverName;
	// signal(SIGINT, signalHandler);
	int requestIndex = 0;
	struct sockaddr_in addr;

	initCommands();
	printServerTitle();
	// create Server 
	createServerSocket();
	configureServerSocket(addr);
	bindServerSocket(addr);
	listenServerSocket();

	// create epoll instance
	createEpoll();

	// in the loop, monitor events and if event on server socket, add new client to epoll,
	// else, handle client event
	while (g_running)
	{
		struct epoll_event events[MAX_EVENTS];
		int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
		if (numEvents == -1 && g_running) {
			throw std::runtime_error("Error epoll_wait");
		}
		for (int i = 0; i < numEvents; i++)
		{
			int clientSocket;
			if (events[i].data.fd == _serverSocket)
			{
				if (handleNewClient(clientSocket) == false)
					continue;
			}
			else
			{
				// handle data from the client socket
				clientSocket = events[i].data.fd;
				std::string buffer = "";
				// If there is no \n, the command is not complete (ctrl+D)
				while (buffer.find("\n") == std::string::npos)
					if (requestIsComplete(clientSocket, buffer) == false)
						break;
				if (buffer.find('\n') != std::string::npos)
				{
					if (buffer.find("PING") == std::string::npos)
						displayRequest(requestIndex, buffer);
					else
						std::cout << "📍" << DIM << " PING!" << RESET << std::endl;
					handleRequest(_clients.at(clientSocket), cleanBuffer(buffer));
					// if the client is authentificated (PASS NICK USER) and not RPL_WELCOMEd
					try
					{
						if (_clients.find(clientSocket) != _clients.end()
							&& _clients.at(clientSocket).isAuthentificated()
							&& _clients.at(clientSocket).getWelcomeState() == 0)
							welcomeClient(clientSocket);
					}
					catch(const std::exception& e)
					{
						std::cout << e.what() << '\n';
					}
					if (std::string(buffer).find("PING") == std::string::npos)
						outputUsersChannels(_clients, _channels);
					SEPARATOR;
				}
			}
		}
	}
	close(_epollFd);
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
	delete _commands["NICK"];
	delete _commands["USER"];
	delete _commands["PING"];
	delete _commands["PASS"];
	delete _commands["JOIN"];
	delete _commands["TOPIC"];
	delete _commands["INVITE"];
	delete _commands["PART"];
	delete _commands["KICK"];
	delete _commands["MODE"];
	delete _commands["PRIVMSG"];
	delete _commands["NOTICE"];
	delete _commands["WHO"];
	delete _commands["QUIT"];
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
std::string							Server::getSupportedParams() const 
{
	std::stringstream	replyStream;

	replyStream << "CHANLIMIT="		<< CHANLIMIT << " ";
	replyStream << "CHANMODES="		<< CHANMODES << " ";
	replyStream << "CHANNELLEN="	<< CHANNELLEN << " ";
	replyStream << "CHANTYPES="		<< CHANTYPES << " ";
	replyStream << "KICKLEN="		<< KICKLEN << " ";
	replyStream << "MODES="			<< MODES << " ";
	replyStream << "NICKLEN="		<< NICKLEN << " ";
	replyStream << "PREFIX="		<< PREFIX << " ";
	replyStream << "TOPICLEN="		<< TOPICLEN << " ";
	replyStream << "USERLEN="		<< USERLEN;

	return (replyStream.str());
}
void								Server::setPort( int port ) { _port = port; };
void								Server::setPassword( std::string password ) { _password = password; };
void								Server::setClients( std::map< int, Client > clients ) { _clients = clients; };
void								Server::setCommands( std::map< std::string, ACommand* > commands ) { _commands = commands; }

/* METHODS ********************************************************************/

// create server socket and set options to ensure port is reusable if server is restarted.
void								Server::createServerSocket()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0)
	{
		perror("Error creating socket");
        throw std::runtime_error("Error creating socket");
    }
	// make sure port is reusable

	int opt =1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Erreur lors de la configuration de setsockopt");
        close(_serverSocket);
	    throw std::runtime_error("Erreur lors de la configuration de setsockopt");
    }

}

// Set options to ensure port is reusable if server is restarted
// and configure address and port for server socket
void	Server::configureServerSocket(struct sockaddr_in& addr)
{
	// set communication type to TCP/IPv4
	addr.sin_family = AF_INET;
	//  listen for connections on all available network
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//set port number 
	addr.sin_port = htons(_port);
}

void								Server::bindServerSocket(const struct sockaddr_in& addr)
{
	int result = bind(_serverSocket, (struct sockaddr*)&addr, sizeof(addr));
    if (result < 0) {
        throw std::runtime_error("Error binding socket");        
    }
}

void								Server::listenServerSocket()
{
	if (listen(_serverSocket, SOMAXCONN) == -1)
		throw std::runtime_error("Could not set server socket to listening state");
}

// create epoll instance and add server socket to it
void								Server::createEpoll()
{
	_epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		throw std::runtime_error("Error creating epoll instance");        
	}
	// add server socket to epoll instance
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = _serverSocket;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSocket, &event) == -1) {
		throw std::runtime_error("Error adding server socket to epoll instance");        
	}
}

void								Server::acceptNewConnection(int& clientSocket)
{
	// accept the connection
	struct sockaddr_in clientAddress;
	socklen_t clientaddrlen = sizeof(clientAddress);
	// int client_socket = accept(_serverSocket, NULL, 0);
	clientSocket = accept(_serverSocket, (struct sockaddr *) &clientAddress, &clientaddrlen);
}



void								Server::addClient( int fd, Client client )
{
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = fd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
		throw std::runtime_error("Error adding client");

	SEPARATOR;
	std::cout	<< YELLOW_BLOC << " Client connecting " << RESET << std::endl;
	std::cout	<< BOLD << "Socket:\t\t" << RESET << client.getClientSocket() << std::endl;
	// _clients[fd] = client;
	// std::cout << "ADDING CLIENT :" << fd << std::endl;
	_clients[fd] = client;
	// _clients.insert( std::make_pair( fd, client ));
	std::cout	<< BOLD << "Clients:\t" << RESET << _clients.size() << std::endl;
}

bool								Server::handleNewClient(int& clientSocket)
{
	acceptNewConnection(clientSocket);	
	if (clientSocket == -1 && g_running)
	{
		std::cout	<< BRED << "Error" << RESET << ": Could not connect client." << std::endl;
		return (false);
	}
	addClient(clientSocket, Client(clientSocket, _serverName));
	return (true);
}

bool								Server::requestIsComplete(const int& clientSocket, std::string& bufferstr)
{
	char buffer[1024] = {};
	int received = recv(clientSocket, buffer, sizeof(buffer), 0);
	bufferstr += buffer;
	// Si la réception est inférieure ou égale à 0, le client s'est déconnecté.
	if (received <= 0) {
		std::cout << "Client disconnected" << std::endl;
		removeClient( clientSocket ); // remove from the client map and close fd
		// break ;
		return (false);
	}
	return (true);
}

void								Server::displayRequest(int& requestIndex, const std::string& buffer)
{
	std::cout	<< HIGHLIGHT << BOLD << " #" << ++requestIndex << " " << RESET
	<< DIM << " Request received " << RESET << std::endl << std::endl;
	std::cout << BOLD << buffer << RESET << std::endl;
	
	return;
}

void								Server::welcomeClient(const int& clientSocket)
{
	Client*			client		= &_clients.at(clientSocket);
	std::string		nickname	= client->getNickname();
	sendNumericReplies(6, clientSocket, \
						RPL_WELCOME(_serverName, nickname).c_str(), \
						RPL_YOURHOST(_serverName, nickname, "1.0").c_str(),
						RPL_CREATED(_serverName, nickname, "in 1942").c_str(), \
						RPL_MYINFO(_serverName, nickname, "1.0", "+i", "+t").c_str(), \
						RPL_ISUPPORT(_serverName, nickname, getSupportedParams()).c_str(), \
						ERR_NOMOTD(_serverName, nickname).c_str());
	sendWelcomeMessage(_clients.at(clientSocket));
	_clients.at(clientSocket).setWelcomeState(true);
	
	return;
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
	// std::cout << "\n[removeClient]\n _client.size:" << _clients.size() << "\n"; 
	if( close( fd ) == -1 )
		throw std::runtime_error("Error when closing fd");
	// _clients[fd].leaveAllChannels();
	// std::cout << RED_BLOC << "Map size before erasing: " << RESET << _clients.size() << std::endl;
	_clients.erase( fd );
	// std::cout << RED_BLOC << "Map size after erasing: " << RESET << _clients.size() << std::endl;
}

// after handling each request, check is any channel in _channels is empty. If so, erase it
void								Server::removeEmptyChannels()
{
	channelMap::iterator it = _channels.begin();
	while (it != _channels.end())
	{
    	if (it->second.getClientMap().empty())
    	    _channels.erase(it++);
		else
    	    it++;
	}
}

// Add all command instances to the server's _commands map
void								Server::initCommands()
{
	_commands["NICK"] = new Nick(&_clients);
	_commands["USER"] = new User(&_clients);
	_commands["PING"] = new Ping(&_clients);
	_commands["PASS"] = new Pass(&_clients, _password);
	_commands["JOIN"] = new Join(&_clients, &_channels);
	_commands["TOPIC"] = new Topic(&_clients, &_channels);
	_commands["INVITE"] = new Invite(&_clients, &_channels);
	_commands["PART"] = new Part(&_clients, &_channels);
	_commands["KICK"] = new Kick(&_clients, &_channels);
	_commands["MODE"]	= new Mode(&_clients, &_channels);
	_commands["PRIVMSG"] = new Privmsg(&_clients, &_channels);
	_commands["NOTICE"] = new Notice(&_clients, &_channels);
	_commands["WHO"] = new Who(&_clients, &_channels);
	_commands["QUIT"] = new Quit(&_clients, &_channels);
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
	int	clientSocket = client.getClientSocket();
	std::istringstream	requestStream(request);
	// std::cout << YELLOW << "commands to run : " << RESET << requestStream.str() << std::endl;
	while(!requestStream.eof() )
	{
		size_t		firstSpace;
		std::string	line;
		std::string	command;
		std::string	parameters = "";

		std::getline(requestStream, line);
		if (line.empty())
			continue ;
		firstSpace = line.find(' ', 0);
		if (firstSpace == std::string::npos)
			command = line;
		else
		{
			// PRINT("extracting command", "");
			command = line.substr(0, firstSpace);
			parameters = line.substr(firstSpace + 1, std::string::npos);
		}
		// if client has been disconnected
		if (_clients.find(clientSocket) == _clients.end())
			break ;
		// else, if command is found
		if (_commands.find(command) != _commands.end())
		{
			// if the password is not set, accept only pass command
			if ( command != "PASS" && client.getPassword().empty())
				continue ;
			_commands[command]->handleRequest(client, parameters);
			removeEmptyChannels();
		}
	}
}

// This function removes \r characters from the buffer.
std::string						Server::cleanBuffer(std::string buffer) const 
{
	// std::cout << "[before cleanBuffer()]\n" << BLUE << buffer << RESET << std::endl;
	while (true)
	{
		size_t pos = buffer.find('\r', 0);
		if (pos == std::string::npos)
			break;
		buffer.erase(pos, 1);
	}
	// std::cout << "[after cleanBuffer()]\n" << YELLOW << buffer << RESET << std::endl;
	return buffer;
}
