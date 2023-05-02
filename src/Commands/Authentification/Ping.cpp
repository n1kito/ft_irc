#include "Ping.hpp"

/* CONSTRUCTORS ***************************************************************/

Ping::Ping() : ACommand()
{
	// std::cout << "Default constructor called" << std::endl;
}

Ping::Ping( std::map< int, Client >* clients ) : ACommand(clients)
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


void	Ping::handleRequest( Client& client, std::string argument )
{
	 std::string message = "";
	std::string ret_parsing = parseArgument(client, argument);
	if (!ret_parsing.empty())
		message = ret_parsing;
	else
		message = PONG_SUCCESS(client.getServerName(), argument);
	send(client.getClientSocket(), message.c_str(), message.length(), 0);
}

void		Ping::parseArgument() {}

std::string	Ping::parseArgument( Client& client, std::string argument )
{
	if (argument.empty())
		return (ERR_NEEDMOREPARAMS(client.getServerName(), "PING"));
	return "";
}

void		Ping::action() {}


