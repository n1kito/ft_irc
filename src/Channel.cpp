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

// getters
std::string						Channel::getName() const { return _name; }
std::string						Channel::getKey() const { return _key; }

std::string						Channel::getTopic() const { return _topic; }
const Channel::clientNickMap&	Channel::getClientMap() const { return _connectedClients; }
const Channel::clientNickMap&	Channel::getOperators() const { return _operators; }

// getters -> channel modes
bool							Channel::isClientLimitMode() const { return _clientLimit > 0; }
bool							Channel::isTopicProtectedMode() const { return _topicIsProtected; }
bool							Channel::isChannelProtectedMode() const { return _channelIsProtected; }

// setters
void							Channel::setTopic(const std::string& newTopic) { _topic = newTopic;}
void							Channel::setKey(const std::string& newKey) { _key = newKey;}


void							Channel::setName(const std::string& newName) { _name = newName; }
void							Channel::setClientLimit(const size_t& limit) { _clientLimit = limit; }
void							Channel::setTopicProtection(const bool& status) { _topicIsProtected = status; }
void							Channel::setChannelProtection(const bool& status) { _channelIsProtected = status; }

void							Channel::addConnectedClient(const Client& clientRef)
{ 
	if (_connectedClients.find(clientRef.getNickname()) == _connectedClients.end())
		_connectedClients[clientRef.getNickname()] = &clientRef;
	
	// message = JOIN_SUCCESS(client.getNickname(), _channelList[_channelList.size() -1]);
	// std::string finalmessage = PRIVMSG(client.getServerName(), _channelList[_channelList.size() -1], message);
	// int numberOfReplies = _topic.empty() ? 3 : 4;
	std::string nickname = clientRef.getNickname();
	std::string server = clientRef.getServerName();

	// if (numberOfReplies == 4)
	broadcastNumericReplies(1, JOIN_MSG(server, _name, nickname).c_str());
	int numberOfReplies = _topic.empty() ? 2 : 4;
	if (numberOfReplies == 2)
		sendNumericReplies(numberOfReplies, clientRef.getClientSocket(), \
							RPL_NAMREPLY,
							RPL_ENDOFNAMES)
	else
		sendNumericReplies(numberOfReplies, clientRef.getClientSocket(), \
							RPL_TOPIC(server, nickname, _name).c_str(), \
							RPL_TOPICWHOTIME(server, nickname, _name, _topic, _nicknameOfTopicSetter).c_str(),
							RPL_NAMREPLY,
							RPL_ENDOFNAMES);
	// else
	// {

	// }

    // 1.  A JOIN message with the client as the message <source> and the channel they have joined as the first parameter of the message.
    // 2. The channel’s topic (with RPL_TOPIC (332)
	// 3. Optionally RPL_TOPICWHOTIME (333)), and no message if the channel does not have a topic.
    // 4. /A list of users currently joined to the channel (with one or more RPL_NAMREPLY (353) numerics followed by a single RPL_ENDOFNAMES (366) numeric). These RPL_NAMREPLY messages sent by the server MUST include the requesting client that has just joined the channel.

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