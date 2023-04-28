#ifndef SERVER_HPP
# define SERVER_HPP

#include "Client.hpp"
#include "ACommand.hpp"
#include "ft_irc.hpp"
#include <map>
#include <sys/epoll.h>
#include <cstdarg> // for va_arg
// #include <cerrno>
#include <vector>
#include <limits>
#include "Nick.hpp"
#include "User.hpp"
#include "Ping.hpp"
#include "Pass.hpp"
#include "Channel.hpp"
#include <ctime>
#include <unistd.h>

#define MAX_EVENTS 10

class Server
{
	public:
		Server(const int& port, const std::string& password, const std::string& serverName);
		~Server();

		int									getPort() const;
		std::string							getPassword() const;
		std::map< int, Client >				getClients() const;
		const std::map< int, Client >*		getClientsPtr() const;
		std::map< std::string, ACommand* >	getCommands() const;
		std::string							getCreationDate() const;

		void								setPort( int port );
		void								setPassword( std::string password );
		void								setClients( std::map< int, Client > clients );
		void								setCommands( std::map< std::string, ACommand* > commands );

		void								addClient( int fd, Client client );
		void								removeClient( int fd );
		void								initCommands();
		void								handleRequest(Client& client, const std::string& request);
		std::string							cleanBuffer(std::string buffer) const;

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
		
		std::string							_getCurrentDate() const;

		int									_port;
		std::string							_password;
		std::vector< Channel >				_channels;
		std::map< int, Client >				_clients;
		std::map< std::string, ACommand* >	_commands;
		std::string							_creationDate;
		std::string							_serverName;
		Server();
};

#endif
