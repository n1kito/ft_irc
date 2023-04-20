#include "Client.hpp"

/* CONSTRUCTORS ***************************************************************/

Client::Client()
{
	// std::cout << "Default constructor called" << std::endl;
}

Client::Client(const Client &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Client::~Client()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Client& Client::operator = (const Client &copyMe)
{
	// (void)copyMe;
	_isRegistered = copyMe._isRegistered;
	_username = copyMe._username;
	_nickname = copyMe._nickname;
	_password = copyMe._password;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/



