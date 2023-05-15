#include "Channel.hpp"

#define TOPICLEN 100

/* CONSTRUCTORS ***************************************************************/

Channel::Channel() :
	_name(""),
	_topic(""),
	_nicknameOfTopicSetter(""),
	_timeTopicWasSet(""),
	_timeChannelWasCreated(getCurrentDate()),
	_key(""),
	_clientLimit(0),
	_inviteOnly(false),
	_topicIsProtected(false),
	_channelIsProtected(false)
{
	// std::cout << "Default constructor called" << std::endl;
}

Channel::Channel( std::string name, const Client& client ) :
	_name(name),
	_topic(""),
	_nicknameOfTopicSetter(""),
	_timeTopicWasSet(""),
	_timeChannelWasCreated(getCurrentDate()),
	_key(""),
	_clientLimit(0),
	_channelIsProtected(false)
{
	addOperator(client);
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
	// TODO: check that all the elements needed are here, because otherwise it breaks stuff
	_connectedClients = copyMe.getClientMap();;
	_operators = copyMe.getOperators();;
	_name = copyMe.getName();
	_key = copyMe.getKey();
	_topic = copyMe.getTopic();
	_timeChannelWasCreated = copyMe.getCreationTime();
	_timeTopicWasSet = copyMe.getTimeTopicWasSet();
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
std::string						Channel::getCreationTime() const { return _timeChannelWasCreated; }
std::string						Channel::getTimeTopicWasSet() const { return _timeTopicWasSet; }

// getters -> channel modes
bool							Channel::addChannelMode(const char& mode, const std::string& parameter)
{
	// i = invite only
	// t = topic protected
	// k = key
	// l = client limit
	if (mode == 'i' || mode == 't' || mode == 'k' || mode == 'l')
	{
		if (modeIs(mode) == false)
			_channelModes[mode] = parameter;
		return true;
	}
	return false;
}
bool							Channel::removeChannelMode(const char& mode)
{
	modeMap::iterator	modeIt = _channelModes.find(mode);
	if (modeIt != _channelModes.end())
	{
		_channelModes.erase(modeIt);
		return true;
	}
	return false;
}
bool							Channel::modeIs(const char& mode)
{
	return _channelModes.find(mode) != _channelModes.end();
}
bool							Channel::modeIs(const std::string& modeStr)
{
	char mode;
	if (modeStr.length() == 1)
		mode = modeStr[0];
	else if (modeStr == "invite-only" )
		mode = 'i';
	else if (modeStr == "topic-protected")
		mode = 't';
	else if (modeStr == "key")
		mode = 'k';
	else if (modeStr == "client-limit")
		mode = 'l';
	return _channelModes.find(mode) != _channelModes.end();
}
std::string						Channel::getModeParameter(const char& mode)
{
	if (_channelModes.find(mode) != _channelModes.end())
		return _channelModes[mode];
	return "";
}
std::string						Channel::listModes() const
{
	std::stringstream		returnStream;

	for (modeMap::const_iterator it = _channelModes.begin(); it != _channelModes.end(); ++it)
	{
		returnStream << (*it).first;
		// if (it != --_channelModes.end())
			// returnStream << ' ';
	}
	return returnStream.str();
}
std::string						Channel::listModeParameters() const
{
	std::string				returnString = "";

	for (modeMap::const_iterator it = _channelModes.begin(); it != _channelModes.end(); ++it)
	{
		returnString += (*it).second;
		if (it != --_channelModes.end())
			returnString += " ";
	}
	return returnString;
}

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
// void							Channel::setKey(const std::string& newKey) { _key = newKey;}


void							Channel::setName(const std::string& newName) { _name = newName; }
// void							Channel::setClientLimit(const size_t& limit) { _clientLimit = limit; }
void							Channel::setTopicProtection(const bool& status) { _topicIsProtected = status; }
void							Channel::setChannelProtection(const bool& status) { _channelIsProtected = status; }

// void							Channel::setInviteOnly(const bool& status) { _inviteOnly = status; }
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
void							Channel::addOperator(const Client& clientRef)
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

void							Channel::broadcastNumericReplies(const size_t numberOfReplies, ...)
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
		if (isClientOperator(*(it->second)))
			usersList += "@";
		usersList += it->second->getNickname();
		if (it != --_connectedClients.end())
			usersList += " ";
		it++;
	}
	return (usersList);
}
