/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:39:44 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:39:45 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/* CONSTRUCTORS ***************************************************************/

Channel::Channel() :
	_name(""),
	_topic(""),
	_nicknameOfTopicSetter(""),
	_timeTopicWasSet(""),
	_timeChannelWasCreated(getCurrentDate())
{}

Channel::Channel(std::string name, const Client& client) :
	_name(name),
	_topic(""),
	_nicknameOfTopicSetter(""),
	_timeTopicWasSet(""),
	_timeChannelWasCreated(getCurrentDate())
{
	addOperator(client);
	addConnectedClient(client, true);
}

Channel::Channel(const Channel &copyMe) { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Channel::~Channel() {}

/* OVERLOADS ******************************************************************/

Channel&						Channel::operator = (const Channel &copyMe)
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
	else if (modeStr == "invite-only")
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
bool							Channel::isClientOperator(const Client& clientRef) const
{
	return _operators.find(clientRef.getNickname()) != _operators.end();
}

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
void							Channel::addConnectedClient(const Client& clientRef, bool isChannelCreator)
{
	if (_connectedClients.find(clientRef.getNickname()) != _connectedClients.end())
		return;

	_connectedClients[clientRef.getNickname()] = &clientRef;
	
	std::string nickname = clientRef.getNickname();
	std::string	username = clientRef.getUsername();
	std::string server = clientRef.getServerName();
	std::string	channel = _name;

	broadcastNumericReply(JOIN_MSG(server, _name, nickname, username).c_str());
	if (isChannelCreator == true)
	{
		addChannelMode('t');
		sendNumericReplies(1, clientRef.getClientSocket(), \
						CSTM_SERVER_MODE_MSG(clientRef.getServerName(), getName(), "t").c_str());
	}
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
		_operators[clientRef.getNickname()] = &clientRef;
}
void							Channel::removeOperator(const std::string& clientNickname)
{
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
			break;		}
	}
}

const std::string						Channel::getUsersList() const
{
	std::string						usersList = "";
	clientNickMap::const_iterator	it = _connectedClients.begin();
	clientNickMap::const_iterator	ite = _connectedClients.end();

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
