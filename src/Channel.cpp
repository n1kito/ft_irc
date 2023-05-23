#include "Channel.hpp"

/* CONSTRUCTORS ***************************************************************/

Channel::Channel() :
	_name(""),
	_topic(""),
	_nicknameOfTopicSetter(""),
	_timeTopicWasSet(""),
	_timeChannelWasCreated(getCurrentDate())
{
	// std::cout << "Default constructor called" << std::endl;
}

Channel::Channel( std::string name, const Client& client ) :
	_name(name),
	_topic(""),
	_nicknameOfTopicSetter(""),
	_timeTopicWasSet(""),
	_timeChannelWasCreated(getCurrentDate())
{
	// sendNumericReplies(2, client.getClientSocket(),
		// std::string(":pouetmania 353 " + client.getNickname() + " = " + getName() + " :" + getUsersList() + "\r\n").c_str(), \
		// std::string(":pouetmania 366 " + client.getNickname() + " " + getName() + " :End of /NAMES list.\r\n").c_str());
		// :nikito!~c@783829BF.B270E442.5F584402.IP JOIN :#channelito
		// :roubaix.fr.epiknet.org 333 nikito #channel coffee 1543326238
		// :roubaix.fr.epiknet.org 353 nikito = #channel :nikito jee @coffee
		// :roubaix.fr.epiknet.org 366 nikito #channel :End of /NAMES list.
	addOperator(client);
	addConnectedClient(client, true);
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
	_connectedClients = copyMe.getClientMap();
	_invitedClients = copyMe.getInvitedClients();
	_operators = copyMe.getOperators();
	_name = copyMe.getName();
	_topic = copyMe.getTopic();
	_nicknameOfTopicSetter = copyMe.getNicknameOfTopicSetter();
	_timeTopicWasSet = copyMe.getTimeTopicWasSet();
	_timeChannelWasCreated = copyMe.getCreationTime();
	_channelModes = copyMe.getChannelModes();
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


void							Channel::sendMessageToChannel(const std::string message, Client& client) const
{
	if (_connectedClients.empty())
		return;
	(void)client;
	for (Channel::clientNickMap::const_iterator it = _connectedClients.begin(); it != _connectedClients.end(); ++it)
		if ((*it).second->getClientSocket() != client.getClientSocket())
			send((*it).second->getClientSocket(), message.c_str(), message.size(), 0);
}

void							Channel::sendMessageToOperators(const std::string message, Client& client) const
{
	if (_operators.empty())
		return;
	for (Channel::clientNickMap::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
		if ((*it).second->getClientSocket() != client.getClientSocket())
			send((*it).second->getClientSocket(), message.c_str(), message.size(), 0);
}

// getters
std::string						Channel::getName() const { return _name; }
std::string						Channel::getKey() const { return getModeParameter("key"); }
std::string						Channel::getTopic() const { return _topic; }
const Channel::clientNickMap&	Channel::getClientMap() const { return _connectedClients; }
const Channel::clientNickMap&	Channel::getOperators() const { return _operators; }
std::string						Channel::getNicknameOfTopicSetter() const { return _nicknameOfTopicSetter; }
std::string						Channel::getCreationTime() const { return _timeChannelWasCreated; }
std::string						Channel::getTimeTopicWasSet() const { return _timeTopicWasSet; }
size_t							Channel::getClientLimit() const
{
	if (modeIs("client-limit"))
		return static_cast< size_t >(std::atoi(getModeParameter("client-limit").c_str()));
	return 0;
}
const Channel::nickVector&		Channel::getInvitedClients() const { return  _invitedClients; }
const Channel::modeMap&			Channel::getChannelModes() const { return _channelModes; }

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
bool							Channel::modeIs(const char& mode) const
{
	return _channelModes.find(mode) != _channelModes.end();
}
bool							Channel::modeIs(const std::string& modeStr) const
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
std::string						Channel::getModeParameter(const char& modeChar) const
{
	if (_channelModes.find(modeChar) != _channelModes.end())
		return _channelModes.at(modeChar);
	return "";
}
std::string						Channel::getModeParameter(const std::string& modeStr) const
{
	char mode;

	if (modeStr.length() == 1)
		return _channelModes.at(modeStr[0]);
	else if (modeStr == "invite-only")
		mode = 'i';
	else if (modeStr == "topic_protected")
		mode = 't';
	else if (modeStr == "key")
		mode = 'k';
	else if (modeStr == "client-limit")
		mode = 'l';
	if (_channelModes.find(mode) != _channelModes.end())
		return _channelModes.at(mode);
	return "";
}
void							Channel::updateMode(const char& mode, const std::string& param) { _channelModes[mode] = param; }
std::string						Channel::listModes() const
{
	std::stringstream		returnStream;

	for (modeMap::const_iterator it = _channelModes.begin(); it != _channelModes.end(); ++it)
	{
		if (it == _channelModes.begin())
			returnStream << "+";
		returnStream << (*it).first;
		// if (it != --_channelModes.end())
			// returnStream << ' ';
	}
	return returnStream.str();
}
std::string						Channel::listModeParameters() const
{
	std::string				returnString = "";
	bool					includingParameters = false;

	for (modeMap::const_iterator it = _channelModes.begin(); it != _channelModes.end(); ++it)
	{
		if ((*it).second.empty() == false)
		{
			if (includingParameters == false)
			{
				returnString += " ";
				includingParameters = true;
			}
			returnString += (*it).second;
			if (it != --_channelModes.end())
				returnString += " ";
		}
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

bool							Channel::isInvited(const std::string& clientNick) const
{
	for (nickVector::const_iterator it = _invitedClients.begin(); it != _invitedClients.end(); ++it)
		if (*it == clientNick)
			return true;		
	return false;
}

// setters
void							Channel::setTopic(const std::string& newTopic) { _topic = newTopic;}
// void							Channel::setKey(const std::string& newKey) { _key = newKey;}


void							Channel::setName(const std::string& newName) { _name = newName; }
// void							Channel::setClientLimit(const size_t& limit) { _clientLimit = limit; }
// void							Channel::setTopicProtection(const bool& status) { _topicIsProtected = status; }
// void							Channel::setChannelProtection(const bool& status) { _channelIsProtected = status; }

// void							Channel::setInviteOnly(const bool& status) { _inviteOnly = status; }
void							Channel::addConnectedClient(const Client& clientRef, bool isChannelCreator)
{
	if (_connectedClients.find(clientRef.getNickname()) != _connectedClients.end())
		return ;

	_connectedClients[clientRef.getNickname()] = &clientRef;
	
	std::string nickname = clientRef.getNickname();
	std::string	username = clientRef.getUsername();
	std::string server = clientRef.getServerName();
	std::string	channel = _name;

	// std::cout << "JOIN_MSG: " << JOIN_MSG(nickname, username, channel) << std::endl;;
	// Let everyone on the Channel know that user has joined
	broadcastNumericReplies(1, JOIN_MSG(server, _name, nickname, username).c_str());
	if (isChannelCreator == true)
	{
		addChannelMode('t');
		// Send a MODE message to let the client know that we decided to add this mode
		sendNumericReplies(1, clientRef.getClientSocket(), \
						CSTM_SERVER_MODE_MSG(clientRef.getServerName(), getName(), "t").c_str());
	}
	if (_topic.empty() == false)
		sendNumericReplies(2, clientRef.getClientSocket(), \
							RPL_TOPIC(server, nickname, channel, _topic).c_str(), \
							RPL_TOPICWHOTIME(server, nickname, channel, _nicknameOfTopicSetter, _timeTopicWasSet).c_str());
	std::cout << "RPL_NAMREPLY: " << RPL_NAMREPLY(server, nickname, channel, getUsersList()) << std::endl;
	std::cout << "RPL_ENDOFNAMES: " << RPL_ENDOFNAMES(server, nickname, channel) << std::endl;
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
	// std::cout << "[add operator]\n";
	if (_operators.find(clientRef.getNickname()) == _operators.end())
		_operators[clientRef.getNickname( )] = &clientRef;
}
void							Channel::removeOperator(const std::string& clientNickname)
{
	// std::cout << "[remove operator]\n";
	if (_operators.find(clientNickname) != _operators.end())
		_operators.erase(clientNickname);
}

void							Channel::setNicknameOfTopicSetter(const std::string& nickname) { _nicknameOfTopicSetter = nickname; }
void							Channel::setTimeTopicWasSet(const std::string& time) { _timeTopicWasSet = time; }

// setters -> channel modes
void							Channel::addInvitedClient(const std::string& clientNick)
{
	if (!isInvited(clientNick))
		_invitedClients.push_back(clientNick);
}

void							Channel::removeInvitedClient(const std::string& clientNick)
{ 
	for (Channel::nickVector::iterator clientNickIt = _invitedClients.begin(); clientNickIt != _invitedClients.end(); ++clientNickIt)
	{
		if (*clientNickIt == clientNick)
		{
			_invitedClients.erase(clientNickIt);
			break ;
		}
	}
}

bool							Channel::checkName(const std::string name)
{
	if ( name.find_first_of(" ,") != std::string::npos
			|| name.find(7) != std::string::npos
			|| name[0] != '#' )
			return false;
	return  true;
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
