#include "Command.hpp"

/* CONSTRUCTORS ***************************************************************/

Command::Command()
{
	// std::cout << "Default constructor called" << std::endl;
}

Command::Command(const Command &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Command::~Command()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Command& Command::operator = (const Command &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/



