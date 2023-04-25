#include "Client.hpp"

/* CONSTRUCTORS ***************************************************************/

// TODO: is this neede or should it be private ?
Client::Client()
{
	// std::cout << "Default constructor called" << std::endl;
}

Client::Client(const int& clientSocket, const std::string& serverName ) :
	_isRegistered(false),
	_passwordStatus(false),
	_clientSocket(clientSocket),
	_serverName(serverName)
{}

// TODO: is this neede or should it be private ?
Client::Client(const Client &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

// TODO: is this neede or should it be private ?
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
std::string		Client::getServerName() const { return _serverName; }
bool			Client::getPasswordStatus() const { return _passwordStatus; }

void			Client::setRegisterState(bool state) { _isRegistered = state; }
void			Client::setUsername(std::string username) { _username = username; }
void			Client::setRealname(std::string realname) { _realname = realname; }
void			Client::setNickname(std::string nickname) { _nickname = nickname; }
void			Client::setPassword(std::string password) { _password = password; }
void			Client::setPasswordStatus(const bool& status) { _passwordStatus = status;}


/* METHODS ********************************************************************/



