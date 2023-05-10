#include "Client.hpp"
// #define PART_MSG(server, channel, nickname) (std::string(":") + server + "!" + nickname + " PART " + channel + " " + nickname + "\r\n")

#define PART_MSG(server, channel, nickname) (std::string(":") + server + "!" + nickname + " PART " + nickname + " " + channel + "\r\n")
//:<nickname> PART <channel> [message]
// :servername PART_RPL nickname #channel :message
// #define JOIN_MSG(server, channel, nickname) (std::string(":") + nickname + "!" + server + " JOIN " + channel + " " +  nickname + "\r\n")

/* CONSTRUCTORS ***************************************************************/

// TODO: is this neede or should it be private ?
Client::Client() :
	// _connectedToChannels(),
	_isRegistered(false),
	_passwordStatus(false),
	_password(""),
	_clientSocket(-1),
	_welcomeState(0),
	_serverName("")
	{}

Client::Client(const int& clientSocket, const std::string& serverName ) :
	// _connectedToChannels(),
	_isRegistered(false),
	_passwordStatus(false),
	_password(""),
	_clientSocket(clientSocket),
	_welcomeState(0),
	_serverName(serverName)
{
	std::cout << "Constructing client with socket " << _clientSocket << std::endl;
}

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
	_connectedToChannels = copyMe.getChannelsMap();
	_isRegistered = copyMe.getRegisterState();
	_clientSocket = copyMe.getClientSocket();
	_username = copyMe.getUsername();
	_nickname = copyMe.getNickname();
	_password = copyMe.getPassword();
	_welcomeState = copyMe.getWelcomeState();
	_passwordStatus = copyMe.getPasswordStatus();
	_realname = copyMe.getRealname();
	_serverName = copyMe.getServerName();
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
bool			Client::getWelcomeState() const { return _welcomeState; }
std::string		Client::getServerName() const { return _serverName; }
bool			Client::getPasswordStatus() const { return _passwordStatus; }
Channel*		Client::getCurrentChannel() const { return _currentChannel; }
const Client::channelsMap&	Client::getChannelsMap() const { return _connectedToChannels; }

void			Client::setRegisterState(bool state) { _isRegistered = state; }
void			Client::setUsername(std::string username) { _username = username; }
void			Client::setRealname(std::string realname) { _realname = realname; }
void			Client::setNickname(std::string nickname) { _nickname = nickname; }
void			Client::setPassword(std::string password) { _password = password; }
void			Client::setWelcomeState(const bool &state) { _welcomeState = state; }
void			Client::setPasswordStatus(const bool& status) { _passwordStatus = status;}

/* METHODS ********************************************************************/

bool			Client::isAuthentificated() const
{
	return (!_nickname.empty() && _isRegistered);
}

void			Client::addChannel(Channel& channelRef)
{
	std::cout << "\n[Add Channel]\n";
	_connectedToChannels[channelRef.getName()] = &channelRef;
	
    channelsMap::iterator it;
    for (it = _connectedToChannels.begin(); it != _connectedToChannels.end(); ++it)
    {
        std::cout << YELLOW << it->second->getName() << "\n";
    }
    std::cout << RESET << "\n";

}

void			Client::leaveAllChannels()
{
	std::cout << "\n[leaveAllChannels]\n";
	channelsMap::iterator it = _connectedToChannels.begin();
	while (it != _connectedToChannels.end())
	{
		std::cout << YELLOW << it->first << "\n";
		it->second->broadcastNumericReply(PART_MSG(_serverName, _nickname, it->first));
		it->second->removeConnectedClient(_nickname);
		it++;
	}

	_connectedToChannels.clear();
	
	std::cout << RESET << "\n";

}

