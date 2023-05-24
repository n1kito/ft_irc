/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:42:42 by jeepark           #+#    #+#             */
/*   Updated: 2023/05/24 13:39:10 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"

/* CONSTRUCTORS ***************************************************************/

Join::Join() : ACommand() {}

Join::Join(std::map< int, Client >* clients) : ACommand(clients) {}

Join::Join(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
	ACommand(clients),
	_channels(channels)
{}

Join::Join(const Join &copyMe) : ACommand(copyMe) { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Join::~Join() {}

/* OVERLOADS ******************************************************************/

Join& Join::operator = (const Join &copyMe) { (void)copyMe; return *this; }

/* METHODS ********************************************************************/
void		Join::parseArgument() {}
void		Join::action() {}

void 		Join::createErrorTooManyChannels(Client const& client, size_t idx)
{
	std::string message;
	while (idx < _channelList.size())
	{
		message += (ERR_TOOMANYCHANNELS(client.getServerName(), client.getNickname(), _channelList[idx]) + "\r\n");
		idx++;
	}
	sendNumericReplies(1, client.getClientSocket(), message.c_str());
	return;
}

void		Join::action(Client &client)
{
	for (size_t i = 0; i < _channelList.size(); i++)
	{
		if (client.getChannelsMap().size() >= CHANLIMIT)
		{
			createErrorTooManyChannels(client, i);
			return;
		}
		std::map<std::string, Channel>::iterator it = _channels->find(_channelList[i]);
		if (it == _channels->end())
		{
			Channel newChannel(_channelList[i], client);
			(*_channels)[_channelList[i]] = newChannel;
			client.addChannel((*_channels)[_channelList[i]]);
		}
		else
		{
			if (it->second.getClientMap().size() >= MAXCLIENTS) {
				sendNumericReplies(1, client.getClientSocket(), (ERR_CHANNELISFULL(client.getServerName(), client.getNickname(), it->second.getName()).c_str()));
				return;
			}
			if (it->second.modeIs("client-limit") && it->second.getClientMap().size() >= it->second.getClientLimit()) {
				sendNumericReplies(1, client.getClientSocket(), (ERR_CHANNELISFULL(client.getServerName(), client.getNickname(), it->second.getName()).c_str()));
				return;
			}
			if (it->second.modeIs("invite-only") && !it->second.isInvited(client.getNickname())) {
				sendNumericReplies(1, client.getClientSocket(), (ERR_INVITEONLYCHAN(client.getServerName(), client.getNickname(), it->second.getName()).c_str()));
				return;
			}
			if (it->second.modeIs("key")) {	
                if (_keyList.empty() || (i < _keyList.size() && it->second.getKey() != _keyList[i]))
				{
					sendNumericReplies(1, client.getClientSocket(), (ERR_BADCHANNELKEY(client.getServerName(), client.getNickname(), it->second.getName()).c_str()));
					return;
				}
			}
			it->second.addConnectedClient(client);
			client.addChannel(it->second);
		}
	}
	return;
}

void		Join::parseArgument(Client& client, std::string& arg)
{
	std::stringstream	argStream(arg);
	std::string			channelArg;
	std::string			keyArg;
	
	std::getline(argStream, channelArg, ' ');
	std::getline(argStream, keyArg);
	
	std::string			buffer;
	std::stringstream	channelStream(channelArg);
	while (std::getline(channelStream, buffer, ','))
	{
		if (buffer.size() < 2 || buffer.size() > CHANNELLEN)
			sendNumericReplies(1, client.getClientSocket(), (ERR_BADCHANMASK(client.getServerName(), buffer)).c_str());
		else if (buffer[0] != '#')
			sendNumericReplies(1, client.getClientSocket(), (ERR_BADCHANAME(client.getServerName(), client.getNickname(), buffer).c_str()));
		else
			_channelList.push_back(buffer);
	}

	std::stringstream	keyStream(keyArg);
	while (std::getline(keyStream, buffer, ','))
		_keyList.push_back(buffer);
	
	return;
}

void		Join::handleRequest(Client &client, std::string arg)
{
	if (arg.empty())
		sendNumericReplies(1, client.getClientSocket(), (ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "JOIN")).c_str());
	else if (arg == "0")
		client.leaveAllChannels();
	else
	{
		parseArgument(client, arg);
		action(client);
	}

	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	std::fill(_keyList.begin(), _keyList.end(), "");
	_keyList.clear();
	
	return;
}
