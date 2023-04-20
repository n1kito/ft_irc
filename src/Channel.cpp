#include "Channel.hpp"

/* CONSTRUCTORS ***************************************************************/

Channel::Channel()
{
	// std::cout << "Default constructor called" << std::endl;
}

Channel::Channel(const Channel &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Channel::~Channel()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Channel& Channel::operator = (const Channel &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/



