#include "Client.hpp"

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
	// std::cout	<< BOLD << "Clients:\t" << RESET << _clients.size() << std::endl;
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
const Client::channelsMap&	Client::getChannelsMap() const { return _connectedToChannels; }
std::string		Client::getUserModes() const { return _userModes.empty() ? _userModes : "+" + _userModes; }

// getters -> channel modes
// TODO: do we want these methods to take a char instead of a string ? Easier
bool			Client::addUserMode(const char& mode)
{
	if (mode == 'i')
	{
		if ((*this).modeIs('i') == false)
			_userModes.push_back('i');
		return true;
	}
	return false;
}
bool			Client::removeUserMode(const char& mode)
{
	size_t	modePositionInString = 0;

	if (mode == 'i')
	{
		modePositionInString = _userModes.find('i');
		if (modePositionInString != std::string::npos)
		{
			_userModes.erase(modePositionInString, 1);
			return true;
		}
	}
	return false;
}
bool			Client::modeIs(const char &mode)
{
	if (mode == 'i')
		return _userModes.find('i') != std::string::npos;
	return false;
}

void			Client::setRegisterState(bool state) { _isRegistered = state; }
void			Client::setUsername(std::string username) { _username = username; }
void			Client::setRealname(std::string realname) { _realname = realname; }
void			Client::setNickname(std::string nickname) { _nickname = nickname; }
void			Client::setPassword(std::string password) { _password = password; }
void			Client::setWelcomeState(const bool &state) { _welcomeState = state; }
void			Client::setPasswordStatus(const bool& status) { _passwordStatus = status;}
void			Client::setInvitationStatus(const bool& status) { _invitationStatus = status; }

/* METHODS ********************************************************************/

bool			Client::isAuthentificated() const
{
	return (!_nickname.empty() && _isRegistered);
}

void			Client::addChannel(Channel& channelRef)
{
	// std::cout << "\n[Add Channel]\n";
	_connectedToChannels[channelRef.getName()] = &channelRef;
	
    // channelsMap::iterator it;
    // for (it = _connectedToChannels.begin(); it != _connectedToChannels.end(); ++it)
    // {
        // std::cout << YELLOW << it->second->getName() << "\n";
    // }
    // std::cout << RESET << "\n";
}

void			Client::removeChannel(const std::string& channelName)
{
	// std::cout << "\n[removeChannel]\n";
	channelsMap::iterator it = _connectedToChannels.find(channelName);
	if (it == _connectedToChannels.end())
		return;
	it->second->removeConnectedClient(_nickname);
	it->second->removeOperator(_nickname);
	_connectedToChannels.erase(channelName);
}


void			Client::leaveAllChannels()
{
	// std::cout << GREEN << "\n[leaveAllChannels]\n";
	channelsMap::iterator itChannel = _connectedToChannels.begin();
	while (itChannel != _connectedToChannels.end())
	{
		// std::cout << YELLOW << itChannel->first << "\n";
		itChannel->second->broadcastNumericReply(PART_MSG(_serverName, _nickname, itChannel->first, ""));
		itChannel->second->removeConnectedClient(_nickname);
		itChannel->second->removeOperator(_nickname);
		itChannel++;
	}
	_connectedToChannels.clear();
	
	// std::cout << RESET << "\n";
}

void			Client::QuitServer(const std::string& message, std::map< std::string, Channel >* channelsMap)
{
	// std::cout << GREEN << "\n[quitServer]\n";
	channelsMap::iterator itChannel = _connectedToChannels.begin();
	while (itChannel != _connectedToChannels.end())
	{
		// std::cout << YELLOW << itChannel->first << "\n";
		itChannel->second->broadcastNumericReply(QUIT_MSG(_serverName, _nickname, itChannel->first, message));
		itChannel->second->removeConnectedClient(_nickname);
		itChannel->second->removeOperator(_nickname);
		itChannel++;
	}
	_connectedToChannels.clear();
	for (std::map< std::string, Channel >::iterator it = channelsMap->begin(); it != channelsMap->end(); ++it)
	{
		std::vector< std::string > invitedClients = it->second.getInvitedClients();
		for (std::vector< std::string >::iterator itInvited = invitedClients.begin(); itInvited != invitedClients.end(); ++itInvited)
			if (*itInvited == _nickname)
				it->second.removeInvitedClient(_nickname);
	}
	
	// std::cout << RESET << "\n";

}


