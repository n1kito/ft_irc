#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include <string>
# include "Client.hpp"
# include <map>
# include <sys/socket.h>
# include <unistd.h>

class ACommand
{
	public:
		typedef std::map< int, Client>	clientMap;
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

		clientMap* _clients;
		// add protected elements here

	private:
		// add private elements here
};

#endif