#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

#include <string>
#include "Client.hpp"
#include <map>
#include <sys/socket.h>
#include <unistd.h>

class ACommand
{
	public:
		ACommand();
		ACommand(std::map<int, Client>* clients);
		ACommand(const ACommand &copyMe);
		virtual ~ACommand();
		ACommand&		operator = (const ACommand &copyMe);
	
		virtual std::string	handleRequest(Client &client, std::string arg) = 0;

	protected:
		virtual void		parseArgument() = 0;
		virtual void		action() = 0;

		std::map<int, Client>* _clients;
		// add protected elements here

	private:
		// add private elements here
};

#endif