#ifndef PASS_HPP
# define PASS_HPP

#include "ft_irc.hpp"
#include "Client.hpp"
#include <string>
#include <map>
#include "ACommand.hpp"
#include "numericReplies.hpp"

class Pass : public ACommand
{
	public:
		typedef std::map< int, Client >	clientMap;

		Pass(const clientMap* clients);
		Pass(const clientMap* clients, const std::string& serverPassword);
		~Pass();

		void parseArgument();
		void action();
		std::string	handleRequest(Client& client, std::string argument);
	
	protected:
		// add protected elements here

	private:
		Pass();
		Pass(const Pass &copyMe);
		Pass&		operator = (const Pass &copyMe);

		std::string _serverPassword;
};

#endif
