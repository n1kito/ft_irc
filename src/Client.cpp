#include "Client.hpp"

/* CONSTRUCTORS ***************************************************************/

Client::Client()
{
	// std::cout << "Default constructor called" << std::endl;
}

Client::Client( int clientSocket ) : _isRegistered(false), _clientSocket(clientSocket) {}

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
	_clientSocket = copyMe.getClientSocket();
	_username = copyMe._username;
	_nickname = copyMe._nickname;
	_password = copyMe._password;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* ACCESSORS ******************************************************************/
bool			Client::getRegisterState() const { return _isRegistered; }
std::string		Client::getUsername() const { return _username; }
std::string		Client::getRealname() const { return _realname; }
std::string		Client::getNickname() const { return _nickname; }
std::string		Client::getPassword() const { return _password; }
int				Client::getClientSocket() const { return _clientSocket; }

void			Client::setRegisterState(bool state) { _isRegistered = state; }
void			Client::setUsername(std::string username) { _username = username; }
void			Client::setRealname(std::string realname) { _realname = realname; }
void			Client::setNickname(std::string nickname) { _nickname = nickname; }
void			Client::setPassword(std::string password) { _password = password; }


/* METHODS ********************************************************************/



