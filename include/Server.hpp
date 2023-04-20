#ifndef SERVER_HPP
# define SERVER_HPP

#include "Client.hpp"
#include "Command.hpp"
#include "ft_irc.hpp"
#include <map>
#include <sys/epoll.h>
// #include <cerrno>

#define MAX_EVENTS 10

class Server
{
	public:
		Server(int port, std::string password);
		~Server();

		int									getPort() const;
		std::string							getPassword() const;
		std::map< int, Client >				getClients() const;
		std::map< std::string, Command >	getCommands() const;

		void								setPort( int port );
		void								setPassword( std::string password );
		void								setClients( std::map< int, Client > clients );
		void								setCommands( std::map< std::string, Command > commands );

		void								addClient( int fd, Client client );
		void								removeClient( int fd );
	protected:
		// add protected elements here

	private:
		// int createServerSocket();
		// void setSocketOption(serverSocket);
		// int bindSocket(serverSocket); 	
		Server(const Server &copyMe);
		Server&		operator = (const Server &copyMe);

		int									_port;
		std::string							_password;
		std::map< int, Client >				_clients;
		std::map< std::string, Command >	_commands;
		// add private elements here
		
		Server();


};

#endif
