/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:39:46 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:39:47 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* CONSTRUCTORS ***************************************************************/

Client::Client() :
	_isRegistered(false),
	_passwordStatus(false),
	_username(""),
	_realname(""),
	_nickname(""),
	_password(""),
	_clientSocket(-1),
	_welcomeState(0),
	_userModes(""),
	_serverName("")
{}

Client::Client(const int& clientSocket, const std::string& serverName) :
	_isRegistered(false),
	_passwordStatus(false),
	_username(""),
	_realname(""),
	_nickname(""),
	_password(""),
	_clientSocket(clientSocket),
	_welcomeState(0),
	_userModes(""),
	_serverName(serverName)
{}

Client::Client(const Client &copyMe) { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Client::~Client() {}

/* OVERLOADS ******************************************************************/

Client& Client::operator = (const Client &copyMe)
{
	_connectedToChannels = copyMe.getChannelsMap();
	_isRegistered = copyMe.getRegisterState();
	_passwordStatus = copyMe.getPasswordStatus();
	_username = copyMe.getUsername();
	_realname = copyMe.getRealname();
	_nickname = copyMe.getNickname();
	_password = copyMe.getPassword();
	_clientSocket = copyMe.getClientSocket();
	_welcomeState = copyMe.getWelcomeState();
	_userModes = copyMe.getUserModes();
	_serverName = copyMe.getServerName();
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

/* METHODS ********************************************************************/

bool			Client::isAuthentificated() const
{
	return (!_nickname.empty() && _isRegistered);
}

void			Client::addChannel(Channel& channelRef)
{
	_connectedToChannels[channelRef.getName()] = &channelRef;
}

void			Client::removeChannel(const std::string& channelName)
{
	channelsMap::iterator it = _connectedToChannels.find(channelName);
	if (it == _connectedToChannels.end())
		return;
	it->second->removeConnectedClient(_nickname);
	it->second->removeOperator(_nickname);
	_connectedToChannels.erase(channelName);
}

void			Client::leaveAllChannels()
{
	channelsMap::iterator itChannel = _connectedToChannels.begin();
	while (itChannel != _connectedToChannels.end())
	{
		itChannel->second->broadcastNumericReply(PART_MSG(_serverName, _nickname, itChannel->first, ""));
		itChannel->second->removeConnectedClient(_nickname);
		itChannel->second->removeOperator(_nickname);
		itChannel++;
	}
	_connectedToChannels.clear();
}

void			Client::quitServer(const std::string& message, std::map< std::string, Channel >* channelsMap)
{
	channelsMap::iterator itChannel = _connectedToChannels.begin();
	while (itChannel != _connectedToChannels.end())
	{
		itChannel->second->broadcastNumericReply(QUIT_MSG(_serverName, _nickname, _username, itChannel->first, message));
		itChannel->second->removeConnectedClient(_nickname);
		itChannel->second->removeOperator(_nickname);
		itChannel++;
	}
	_connectedToChannels.clear();
	for (std::map< std::string, Channel >::iterator it = channelsMap->begin(); it != channelsMap->end(); ++it)
		it->second.removeInvitedClient(_nickname);
}

void			Client::quitServer(const std::string& message, const std::map< std::string, Channel* >* channelsMap)
{
	channelsMap::iterator itChannel = _connectedToChannels.begin();
	while (itChannel != _connectedToChannels.end())
	{
		itChannel->second->broadcastNumericReply(QUIT_MSG(_serverName, _nickname, _username, itChannel->first, message));
		itChannel->second->removeConnectedClient(_nickname);
		itChannel->second->removeOperator(_nickname);
		itChannel++;
	}
	_connectedToChannels.clear();
	for (std::map< std::string, Channel* >::const_iterator it = channelsMap->begin(); it != channelsMap->end(); ++it)
		it->second->removeInvitedClient(_nickname);
}