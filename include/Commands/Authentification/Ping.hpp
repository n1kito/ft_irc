#ifndef PING_HPP
# define PING_HPP

#include "ACommand.hpp"
#include "ft_irc.hpp"
#include "numericReplies.hpp"

class Ping : public ACommand
{
	public:
		Ping(const Ping &copyMe);
		Ping( std::map< int, Client >* clients );
		
		~Ping();
		Ping&		operator = (const Ping &copyMe);
	
	protected:

		void	handleRequest( Client& client, std::string argument );
		void		parseArgument();
		std::string	parseArgument( Client& client, std::string argument );
		void		action();
		// add protected elements here

	private:
		Ping();
		// add private elements here
};

#endif
