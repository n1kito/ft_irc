#ifndef SERVER_HPP
# define SERVER_HPP

#include "Client.hpp"
#include "ACommand.hpp"
#include "ft_irc.hpp"
#include <map>
#include <iomanip>
#include <sys/epoll.h>
#include <cstdarg> // for va_arg
// #include <cerrno>
#include <vector>
#include <limits>
#include "Nick.hpp"
#include "User.hpp"
#include "Ping.hpp"
#include "Pass.hpp"
#include "Topic.hpp"
#include "Invite.hpp"
#include "Channel.hpp"
#include "Join.hpp"
#include "Part.hpp"
#include "Mode.hpp"
#include "Privmsg.hpp"
#include "Notice.hpp"
#include "Quit.hpp"
#include "Kick.hpp"
#include "Who.hpp"

#include <ctime>
#include <unistd.h>
#include <csignal>

#define MAX_EVENTS 10

class Server
{
	public:
		typedef std::map< int, Client >				clientMap;
		typedef std::map< std::string, Channel >	channelMap;
		typedef std::map< std::string, ACommand* >	commandMap;
		
		Server();
		~Server();

		void								launch(const int& port, const std::string& password, const std::string& serverName);
		int									getPort() const;
		std::string							getPassword() const;
		clientMap							getClients() const;
		const clientMap*					getClientsPtr() const;
		commandMap							getCommands() const;
		std::string							getCreationDate() const;
		std::string							getSupportedParams() const;

		void								setPort( int port );
		void								setPassword( std::string password );
		void								setClients( clientMap clients );
		void								setCommands( commandMap commands );
		
		void								createServerSocket();
		void								configureServerSocket(struct sockaddr_in& addr);
		void								bindServerSocket(const struct sockaddr_in& addr);
		void								listenServerSocket();
		void								createEpoll();
		void								acceptNewConnection(int& clientSocket);
		void								addClient( int fd, Client client );
		bool								handleNewClient(int& clientSocket);
		bool								requestIsComplete(const int& clientSocket, std::string& bufferstr);
		void								displayRequest(int& requestIndex, const std::string& buffer);
		void								welcomeClient(const int& clientSocket);
		void								removeClient( int fd);
		void								removeEmptyChannels();
		void								initCommands();
		void								handleRequest(Client& client, const std::string& request);
		std::string							cleanBuffer(std::string buffer) const;
		// void								signalHandler(int signal);

		// void								sendNumericReplies(const Client& target, const int count, ...);


	
	protected:
		// add protected elements here

	private:
		// int createServerSocket();
		// void setSocketOption(serverSocket);
		// int bindSocket(serverSocket); 	
		Server(const Server &copyMe);
		Server&		operator = (const Server &copyMe);
		Client&		operator [] (const int fd);
	
		int									_port;
		int									_serverSocket;
		int									_epollFd;
		std::string							_password;
		channelMap							_channels;
		clientMap							_clients;
		commandMap							_commands;
		std::string							_creationDate;
		std::string							_serverName;
};

// void	sendNumericReplies(const size_t& numberOfReplies, const size_t clientFd, ...);


#endif
