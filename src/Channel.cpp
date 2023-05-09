#include "Channel.hpp"

#define TOPICLEN 100

/* CONSTRUCTORS ***************************************************************/

Channel::Channel() :
	_name(""),
	_topic(""),
	_key(""),
	_clientLimit(0),
	_topicIsProtected(false),
	_channelIsProtected(false)
{
	// std::cout << "Default constructor called" << std::endl;
}

Channel::Channel( std::string name, const Client& client ) :
	_name(name),
	_topic(""),
	_key(""),
	_clientLimit(0),
	_topicIsProtected(false),
	_channelIsProtected(false)
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
	_key = copyMe.getKey();
	_topic = copyMe.getTopic();
	_clientLimit = copyMe.isClientLimitMode();
	_topicIsProtected = copyMe.isTopicProtectedMode();
	_channelIsProtected = copyMe.isChannelProtectedMode();

	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/

void							Channel::broadcastNumericReply(const std::string message) const
{
	if (_connectedClients.empty())
		return;
	for (Channel::clientNickMap::const_iterator it = _connectedClients.begin(); it != _connectedClients.end(); ++it)
		send((*it).second->getClientSocket(), message.c_str(), message.size(), 0);
}

// getters
std::string						Channel::getName() const { return _name; }
std::string						Channel::getKey() const { return _key; }

std::string						Channel::getTopic() const { return _topic; }
const Channel::clientNickMap&	Channel::getClientMap() const { return _connectedClients; }
const Channel::clientNickMap&	Channel::getOperators() const { return _operators; }
std::string						Channel::getNicknameOfTopicSetter() const { return _nicknameOfTopicSetter; }
std::string						Channel::getTimeTopicWasSet() const { return _timeTopicWasSet; }

// getters -> channel modes
bool							Channel::isClientLimitMode() const { return _clientLimit > 0; }
bool							Channel::isTopicProtectedMode() const { return _topicIsProtected; }
bool							Channel::isChannelProtectedMode() const { return _channelIsProtected; }
bool							Channel::isInviteOnly() const { return _inviteOnly; }

// checkers
// Checks if a client is an operator of the Channel
bool							Channel::isClientOperator(const Client& clientRef) const
{
	return _operators.find(clientRef.getNickname()) != _operators.end();
}

// Checks if a client is connected to the Channel
bool							Channel::isClientConnected(const Client& clientRef) const
{
	return _connectedClients.find(clientRef.getNickname()) != _connectedClients.end();
}


// setters
void							Channel::setTopic(const std::string& newTopic) { _topic = newTopic;}
void							Channel::setKey(const std::string& newKey) { _key = newKey;}


void							Channel::setName(const std::string& newName) { _name = newName; }
void							Channel::setClientLimit(const size_t& limit) { _clientLimit = limit; }
void							Channel::setTopicProtection(const bool& status) { _topicIsProtected = status; }
void							Channel::setChannelProtection(const bool& status) { _channelIsProtected = status; }

void							Channel::setInviteOnly(const bool& status) { _inviteOnly = status; }
void							Channel::addConnectedClient(const Client& clientRef)
{ 
	//TODO: what is this condition for, is it necessary ?    
	if (_connectedClients.find(clientRef.getNickname()) == _connectedClients.end())
		_connectedClients[clientRef.getNickname()] = &clientRef;

	std::string nickname = clientRef.getNickname();
	std::string server = clientRef.getServerName();
	std::string	channel = _name;
	
	// Let everyone on the Channel know that user has joined
	broadcastNumericReplies(1, JOIN_MSG(server, _name, nickname).c_str());
	if (_topic.empty() == false)
		sendNumericReplies(2, clientRef.getClientSocket(), \
							RPL_TOPIC(server, nickname, channel, _topic).c_str(), \
							RPL_TOPICWHOTIME(server, nickname, channel, _nicknameOfTopicSetter, _timeTopicWasSet).c_str());
	sendNumericReplies(2, clientRef.getClientSocket(), \
					RPL_NAMREPLY(server, nickname, channel, getUsersList()).c_str(), \
					RPL_ENDOFNAMES(server, nickname, channel).c_str());
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

void							Channel::setNicknameOfTopicSetter(const std::string& nickname) { _nicknameOfTopicSetter = nickname; }
void							Channel::setTimeTopicWasSet(const std::string& time) { _timeTopicWasSet = time; }

// setters -> channel modes
void							Channel::addInvitedClient(const std::string& clientNick)
{
	_invitedClients.push_back(clientNick);
}

void							Channel::removeInvitedClient(const std::string& clientNick)
{
	Channel::nickVector::iterator clientNickIt = _invitedClients.begin();
	for (; clientNickIt != _invitedClients.end(); ++clientNickIt)
		if (*clientNickIt == clientNick)
			_invitedClients.erase(clientNickIt);
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

void							Channel::broadcastNumericReplies(const size_t& numberOfReplies, ...)
{
	std::va_list	messages;

	va_start(messages, numberOfReplies);
	if (numberOfReplies == 0)
		return;
	for (size_t i = 0; i < numberOfReplies; ++i)
	{
		std::string	message(va_arg(messages, char*));
		for (Channel::clientNickMap::iterator it = _connectedClients.begin(); it != _connectedClients.end(); ++it)
			send(it->second->getClientSocket(), message.c_str(), message.length(), 0);
	}
	va_end(messages);
}

std::string						Channel::getUsersList()
{
	std::string				usersList = "";
	clientNickMap::iterator it = _connectedClients.begin();
	clientNickMap::iterator ite = _connectedClients.end();

	while (it != ite)
	{
		// TODO: handle prefixes
		// if (client is operator)
		// usersList += "@";
		// else if (client is founder)
		// userList += "-";
		usersList += it->second->getNickname();
		if (it != --_connectedClients.end())
			usersList += " ";
		it++;
	}
	return (usersList);
}
