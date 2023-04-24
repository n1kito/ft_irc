#include "User.hpp"

/* CONSTRUCTORS ***************************************************************/

User::User() : ACommand()
{
	// std::cout << "Default constructor called" << std::endl;
}

User::User(const User &copyMe) : ACommand()
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
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* ACCESSORS ******************************************************************/

std::string	User::getUsername() const { return _username; }
std::string	User::getRealname() const { return _realname; }
void		User::setUsername( std::string username ) { _username = username; }
void		User::setRealname( std::string realname ) { _realname = realname; }

/* METHODS ********************************************************************/

const char*	User::handleRequest( Client& client, std::string argument )
{
	const char* ret_parsing = parseArgument(argument);
	if (ret_parsing)
		return ret_parsing;

	const char* ret_action = action(client, _username, _realname);
	if (ret_action)
		return ret_action;
	
	return NULL;
}

const char*	User::parseArgument( std::string argument )
{
	std::stringstream	iss;
	std::string			line;
	
	iss << argument;

	while (getline( iss, line, ' ' ) && line.empty());
	_username = line;


	// first ==> username
	// second ==> "0 *" ==> 
	// last ==> string with spaces (realname) 
	while (getline( iss, line, ' ' ))
	{
		if (line.empty())
			continue ;

	}
	return NULL;
}

const char*	User::action( Client& client, std::string username, std::string realname )
{
	if (client.getRegisterState())
		return (ERR_ALREADYREGISTERED("server", client.getNickname()));
	if (username.empty())
		return (ERR_NEEDMOREPARAMS("server", client.getNickname(), "USER"));
	client.setUsername(username);
	client.setRealname(realname);

	return NULL;
}



