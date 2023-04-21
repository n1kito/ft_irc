#include "User.hpp"

/* CONSTRUCTORS ***************************************************************/

User::User()
{
	// std::cout << "Default constructor called" << std::endl;
}

User::User(const User &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

User::~User()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

User& User::operator = (const User &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/

char*	User::handleRequest( Client& client, std::string argument )
{
	char* ret_parsing = parseArgument(argument);
	if (ret_parsing)
		return ret_parsing;

	char* ret_action = action(client, _username, _realname);
	if (ret_action)
		return ret_action;
	
	return NULL;
}

char*	User::parseArgument( std::string argument )
{

}
char*	User::action( Client& client, std::string username, std::string realname )
{
	if (client.getRegisterState())
		return (ERR_ALREADYREGISTERED("server", client.getNickname()));
	if (username.empty())
		return (ERR_NEEDMOREPARAMS("server", client.getNickname(), "USER"));
	client.setUsername(username);
	client.setRealname(realname);

	return NULL;
}



