#include "Channel.hpp"

#define TOPICLEN 100

/* CONSTRUCTORS ***************************************************************/

Channel::Channel()
{
	// std::cout << "Default constructor called" << std::endl;
}

Channel::Channel( std::string name, const Client& client ) :
	_name(name),
	_clientLimit(0),
	_topicIsProtected(false)
{
	addConnectedClient(client);
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
	_connectedClients = copyMe.getClientMap();;
	_operators = copyMe.getOperators();;
	_name = copyMe.getName();
	_topic = copyMe.getTopic();
	_clientLimit = copyMe.isClientLimitMode();
	_topicIsProtected = copyMe.isTopicProtectedMode();

	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/

// getters
std::string						Channel::getName() const { return _name; }
std::string						Channel::getTopic() const { return _topic; }
const Channel::clientNickMap&	Channel::getClientMap() const { return _connectedClients; }
const Channel::clientNickMap&	Channel::getOperators() const { return _operators; }

// getters -> channel modes
bool							Channel::isClientLimitMode() const { return _clientLimit > 0; }
bool							Channel::isTopicProtectedMode() const { return _topicIsProtected; }

// checkers
bool							Channel::clientIsOperator(const Client& clientRef)
{
	return _operators.find(clientRef.getNickname()) != _operators.end();
}

// setters
void							Channel::setTopic(const std::string& newTopic)
{
	_topic = newTopic; 

}

void							Channel::setName(const std::string& newName) { _name = newName; }
void							Channel::setClientLimit(const size_t& limit) { _clientLimit = limit; }
void							Channel::setTopicProtection(const bool& status) { _topicIsProtected = status; }
void							Channel::addConnectedClient(const Client& clientRef)
{ 
	if (_connectedClients.find(clientRef.getNickname()) == _connectedClients.end())
		_connectedClients[clientRef.getNickname()] = &clientRef;
}
void							Channel::removeConnectedClient(const std::string& clientNickname)
{
	if (_connectedClients.find(clientNickname) != _connectedClients.end())
		_connectedClients.erase(clientNickname);
}
void							Channel::addOperator(Client& clientRef)
{ 
	if (_operators.find(clientRef.getNickname()) == _operators.end())
		_operators[clientRef.getNickname( )] = &clientRef;
	// TODO: send numeric reply ?
}
void							Channel::removeOperator(const std::string& clientNickname)
{
	if (_operators.find(clientNickname) != _operators.end())
		_operators.erase(clientNickname);
	// TODO: send numeric reply ?
}


bool							Channel::checkName(const std::string name)
{
	if ( name.find_first_of(" ,") != std::string::npos
			|| name.find(7) != std::string::npos
			|| name[0] != '#' )
			return false;
	return  true;
}

bool							Channel::checkTopic(const std::string arg)
{
	if (arg.size() > TOPICLEN)
		return false;
	return true;
}

