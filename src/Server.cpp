/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:39:51 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:39:52 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cerrno>
#include "numericReplies.hpp"
#include "ft_irc.hpp"

Server::Server() {}

void		Server::launch(const int& port, const std::string& password, const std::string& serverName)
{
	int				 	requestIndex = 0;
	struct sockaddr_in	addr;

	_port = port;
	_password = password;
	_creationDate = getCurrentDate();
	_serverName = serverName;

	initCommands();
	printServerTitle();
	createServerSocket();
	configureServerSocket(addr);
	bindServerSocket(addr);
	listenServerSocket();
	createEpoll();
	// In the loop, monitor events and if event on server socket, add new client to epoll,
	// else, handle client event
	while (g_running)
	{
		struct epoll_event events[MAX_EVENTS];
		int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);
		if (numEvents == -1 && g_running)
			throw std::runtime_error("Error epoll_wait");
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
				// Handle data from the client socket
				clientSocket = events[i].data.fd;
				std::string buffer = "";
				// If there is no \n, the command is not complete (ctrl+D)
				while (buffer.find("\n") == std::string::npos)
					if (requestIsComplete(clientSocket, buffer) == false)
						break;
				// Handle request 
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

Server::Server(const Server &copyMe) { *this = copyMe; }

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

Server& 							Server::operator = (const Server &copyMe)
{
	(void)copyMe;
	return *this;
}

Client& 							Server::operator[](const int fd) { return _clients[fd]; }

/* ACCESSORS ******************************************************************/

std::string							Server::getPassword() const { return _password; }
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

/* METHODS ********************************************************************/

// Create server socket and set options to ensure port is reusable if server is restarted.
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
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
        perror("Erreur lors de la configuration de setsockopt");
        close(_serverSocket);
	    throw std::runtime_error("Erreur lors de la configuration de setsockopt");
    }
}

// Set options to ensure port is reusable if server is restarted
// and configure address and port for server socket
void								Server::configureServerSocket(struct sockaddr_in& addr)
{
	// set communication type to TCP/IPv4
	addr.sin_family = AF_INET;
	// listen for connections on all available network
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// set port number 
	addr.sin_port = htons(_port);
}

void								Server::bindServerSocket(const struct sockaddr_in& addr)
{
	int result = bind(_serverSocket, (struct sockaddr*)&addr, sizeof(addr));
    if (result < 0)
        throw std::runtime_error("Error binding socket");        
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
	if (_epollFd == -1)
		throw std::runtime_error("Error creating epoll instance");        

	// add server socket to epoll instance
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = _serverSocket;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSocket, &event) == -1)
		throw std::runtime_error("Error adding server socket to epoll instance");        
}

void								Server::acceptNewConnection(int& clientSocket)
{
	struct sockaddr_in clientAddress;
	socklen_t clientaddrlen = sizeof(clientAddress);
	clientSocket = accept(_serverSocket, (struct sockaddr *) &clientAddress, &clientaddrlen);
}


void								Server::addClient(int fd, Client client)
{
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = fd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
		throw std::runtime_error("Error adding client");

	std::cout	<< YELLOW_BLOC << " + " << RESET << DIM << " Client connecting " << RESET << std::endl << std::endl;
	std::cout	<< BOLD << "Socket:\t\t" << RESET << client.getClientSocket() << std::endl;
	_clients[fd] = client;
	std::cout	<< BOLD << "Clients:\t" << RESET << _clients.size() << std::endl;
	SEPARATOR;
}

bool								Server::handleNewClient(int& clientSocket)
{
	acceptNewConnection(clientSocket);	
	if (clientSocket == -1 && g_running)
	{
		std::cout	<< BRED << "Error" << RESET << ": Could not connect client." << std::endl;
		return (false);
	}
	if (_clients.size() >= MAXCONNECTED)
	{
		std::cout	<< BRED << "Error" << RESET << ": Too many clients connected already" << std::endl;
		sendNumericReplies(1, clientSocket, std::string(":" + _serverName + " 999  :Too many clients connected already\r\n").c_str());
		if(close(clientSocket) == -1)
			throw std::runtime_error("Error when closing fd");
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
	// If received <= 0, client disconnected
	if (received <= 0)
	{
		std::cout << "Client disconnected" << std::endl;
		Client*	clientToRemove = &(_clients[clientSocket]);
		clientToRemove->quitServer("disconnected", &(clientToRemove->getChannelsMap()));
		removeClient(clientSocket);
		removeEmptyChannels();
		return (false);
	}
	return (true);
}

void								Server::displayRequest(int& requestIndex, const std::string& buffer)
{
	std::cout	<< HIGHLIGHT << BOLD << " #" << ++requestIndex << " " << RESET
	<< DIM << " Request received " << RESET << std::endl << std::endl;
	std::cout << BOLD << buffer << RESET << std::endl;
}

void								Server::welcomeClient(const int& clientSocket)
{
	Client*			client		= &_clients.at(clientSocket);
	std::string		nickname	= client->getNickname();
	sendNumericReplies(6, clientSocket, \
						RPL_WELCOME(_serverName, nickname).c_str(), \
						RPL_YOURHOST(_serverName, nickname, "1.0").c_str(),
						RPL_CREATED(_serverName, nickname, "in 1942").c_str(), \
						RPL_MYINFO(_serverName, nickname, "1.0", "+i", "+it +kl").c_str(), \
						RPL_ISUPPORT(_serverName, nickname, getSupportedParams()).c_str(), \
						ERR_NOMOTD(_serverName, nickname).c_str());
	sendWelcomeMessage(_clients.at(clientSocket));
	_clients.at(clientSocket).setWelcomeState(true);
}

void								Server::removeClient(int fd)
{
	if(close(fd) == -1)
		throw std::runtime_error("Error when closing fd");
	_clients.erase(fd);
}

// after handling each request, check is any channel in _channels is empty.
// If so, erase it.
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
	int	clientSocket = client.getClientSocket();
	std::istringstream	requestStream(request);
	while(!requestStream.eof())
	{
		size_t		firstSpace;
		std::string	line;
		std::string	command;
		std::string	parameters = "";

		std::getline(requestStream, line);
		if (line.empty())
			continue;
		firstSpace = line.find(' ', 0);
		if (firstSpace == std::string::npos)
			command = line;
		else
		{
			command = line.substr(0, firstSpace);
			parameters = line.substr(firstSpace + 1, std::string::npos);
		}
		// if client has been disconnected
		if (_clients.find(clientSocket) == _clients.end())
			break;		// else, if command is found
		if (_commands.find(command) != _commands.end())
		{
			// if the password is not set, accept only pass command
			if (command != "PASS" && client.getPassword().empty())
				continue;
			if (client.isAuthentificated() ||
				command == "NICK" ||
				command == "USER" ||
				command == "PASS")
				_commands[command]->handleRequest(client, parameters);
			removeEmptyChannels();
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
