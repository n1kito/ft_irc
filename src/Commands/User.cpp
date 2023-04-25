#include "User.hpp"

/* CONSTRUCTORS ***************************************************************/

User::User() : ACommand()
{
	// std::cout << "Default constructor called" << std::endl;
}

User::User( const std::map< int, Client >* clients ) : ACommand(clients)
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

std::string	User::handleRequest( Client& client, std::string argument )
{
	// send(client.getClientSocket(), "", reply.length(), 0);
	std::string ret_parsing = parseArgument(client, argument);
	if (!ret_parsing.empty())
		return ret_parsing;

	std::string ret_action = action(client, _username, _realname);
	if (!ret_action.empty())
		return ret_action;
	
	client.setRegisterState(true);
	return USER_SUCCESS("server", client.getNickname());
	// return ":server: User created successfully!\r\n";
}

void		User::parseArgument() {}

std::string	User::parseArgument( Client& client, std::string argument )
{
	std::stringstream	iss(argument);
	
	iss >> _username;
	iss.ignore(std::numeric_limits< std::streamsize >::max(), ' ');
	if ( _username.size() > USERLEN )
		_username.erase(_username.at(USERLEN));

	// hostname and servername are typically ignored when user comes from a client
	iss.ignore(std::numeric_limits< std::streamsize >::max(), ' ');
	iss.ignore(std::numeric_limits< std::streamsize >::max(), ' ');

	iss >> _realname;
	if (_realname[0] != ':')
		_realname = client.getNickname();

	if  (!iss.good())
		return (ERR_NEEDMOREPARAMS("server", client.getNickname(), "USER"));
	// first ==> username
	// second ==> hostname
	// third ==> servername
	// last ==> string with spaces (realname) 
	return "";
}

void		User::action() {}

std::string	User::action( Client& client, std::string username, std::string realname )
{
	if (client.getRegisterState())
		return (ERR_ALREADYREGISTERED("server", client.getNickname()));
	if (username.empty())
		return (ERR_NEEDMOREPARAMS("server", client.getNickname(), "USER"));
	client.setUsername(username);
	client.setRealname(realname);

	std::cout	<< RED << "Username : " << RESET
				<< client.getUsername() << " | "
				<< "Realname : " << client.getRealname() << std::endl;

	return "";
}
