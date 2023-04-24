#include "ACommand.hpp"

/* CONSTRUCTORS ***************************************************************/

ACommand::ACommand()
{
	// std::cout << "Default constructor called" << std::endl;
}

ACommand::ACommand(const std::map<int, Client>* clients) : _clients(clients) {}

ACommand::ACommand(const ACommand &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

ACommand::~ACommand()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

ACommand& ACommand::operator = (const ACommand &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/



