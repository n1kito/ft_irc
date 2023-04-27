#include "Ping.hpp"

/* CONSTRUCTORS ***************************************************************/

Ping::Ping() : ACommand()
{
	// std::cout << "Default constructor called" << std::endl;
}

Ping::Ping( const std::map< int, Client >* clients ) : ACommand(clients)
{
	// std::cout << "Default constructor called" << std::endl;
}

Ping::Ping(const Ping &copyMe) : ACommand()
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Ping::~Ping()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Ping& Ping::operator = (const Ping &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/


std::string	Ping::handleRequest( Client& client, std::string argument )
{
	std::string ret_parsing = parseArgument(client, argument);
	if (!ret_parsing.empty())
		return ret_parsing;
	return (PONG_SUCCESS(argument));	
}

void		Ping::parseArgument() {}

std::string	Ping::parseArgument( Client& client, std::string argument )
{
	if (argument.empty())
		return (ERR_NEEDMOREPARAMS("server", client.getNickname(), "PING"));
	return "";
}

void		Ping::action() {}


