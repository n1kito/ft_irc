#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include <string>
# include "Client.hpp"
# include <map>
# include <sys/socket.h>
# include <unistd.h>
# include "numericReplies.hpp"

class Client;

class ACommand
{
	public:
		typedef std::map< int, Client >	clientMap;
		
		ACommand();
		ACommand(clientMap* clients);
		ACommand(const ACommand &copyMe);
		virtual ~ACommand();
		ACommand&			operator = (const ACommand &copyMe);
	
		virtual void		handleRequest(Client &client, std::string arg) = 0;

		// getters
		Client*				getClientByNickname(const std::string& nickname);

	protected:
		virtual void		parseArgument() = 0;
		virtual void		action() = 0;
		void				killClient(int fd, std::string previousMsg, std::string errorMsg);

		clientMap* 			_clients;
};

#endif