/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli <cgosseli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 17:02:15 by mjallada          #+#    #+#             */
/*   Updated: 2023/05/11 15:29:23 by cgosseli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kick.hpp"

/* CONSTRUCTORS ***************************************************************/

Kick::Kick() : ACommand() {}

Kick::Kick(std::map< int, Client >* clients) : ACommand(clients) {}


Kick::Kick(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
	ACommand(clients),
	_channelMap(channels)
{
}

Kick::Kick(const Kick &copyMe) : ACommand(copyMe)
{
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Kick::~Kick()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Kick& Kick::operator = (const Kick &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Kick::parseArgument() {}
void		Kick::action() {}

void	Kick::handleRequest(Client &client, std::string arg)
{
	std::cout << GREEN << "[KICK - handleRequest]\n" << RESET;

	std::string message = "";

	if (arg.empty())
		message = ERR_NEEDMOREPARAMS(client.getServerName(), "KICK");
	else
	{
		std::string parseResult = parseArgument(client, arg);
		if (!parseResult.empty())
			message = parseResult;
		else
			message = action(client);
	}
	send(client.getClientSocket(), message.c_str(), message.length(), 0);

	// cleaning
	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	std::fill(_userList.begin(), _userList.end(), "");
	_userList.clear();
}

std::string	Kick::parseArgument(Client &client, std::string& arg)
{
	std::cout << GREEN << "[KICK - parseArgument]\n" << RESET;

	std::stringstream	argStream(arg);
	std::string			channels	=	"";
	std::string			users		=	"";
	std::string			buffer		=	"";


	std::getline(argStream, channels, ' ');
	std::getline(argStream, users, ' ');
	std::getline(argStream, _kickReason);

	std::cout << YELLOW << "|" << users << "|" << RESET << std::endl;
	if (users.empty())
		return ERR_NEEDMOREPARAMS(client.getServerName(), "KICK");

	if (!_kickReason.empty() && _kickReason.at(0) == ':') // can it throw an exception  ?
		_kickReason.erase(0, 1);
	
	// init vector of channels
	std::stringstream	channelsStream(channels);
	while (std::getline(channelsStream, buffer, ','))
	{
		leftTrim(buffer, " \r\t\n");
		if (!buffer.empty())
			_channelList.push_back(buffer);
	}

	// init vector of users
	std::stringstream	usersStream(users);
	while (std::getline(usersStream, buffer, ','))
	{
		leftTrim(buffer, " \r\t\n");
		if (!buffer.empty())
			_userList.push_back(buffer);
	}

	// DEBUG mode on **********************************************************
	std::cout		<< RED << "Channels list: " << RESET;
	for (std::vector< std::string >::iterator it = _channelList.begin(); it != _channelList.end(); ++it)
		std::cout	<< *it << " ";
	std::cout		<< std::endl;

	std::cout		<< RED << "Users list: " << RESET;
	for (std::vector< std::string >::iterator it = _userList.begin(); it != _userList.end(); ++it)
		std::cout	<< *it << " ";
	std::cout		<< std::endl;

	std::cout 		<< RED << "Kick reason: " << RESET << _kickReason << std::endl;
	// ************************************************************************

	return "";
}

std::string Kick::action(Client &client)
{
	std::cout << GREEN << "[KICK - action]\n" << RESET;

	std::string	message = "";

	for (std::vector< std::string >::iterator it = _channelList.begin(); it != _channelList.end(); ++it)
	{
		// check if the channel exists and if it does keep its pos in the channel map
		channelMap::iterator posInChannelMap	= _channelMap->find(*it);
		if (posInChannelMap == _channelMap->end())
			message += ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), *it);
		else
		{
			std::cout << RED << "The print debugger was here: " << RESET << "Kick.cpp:149" << std::endl;
			// get the client map of the channel found and check if the client that wants to kick someone exists
			if (!posInChannelMap->second.isClientConnected(client))
				message += ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), *it);
			// else if (client not privilege)
			// 	message += ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), *it);
			else
			{
				std::cout << RED << "The print debugger was here: " << RESET << "Kick.cpp:157" << std::endl;
				for (std::vector< std::string >::iterator it = _userList.begin(); it != _userList.end(); ++it)
				{
					// get the client information of the user to kick
					std::map< std::string, const Client* >::const_iterator	posInClientMap	=  posInChannelMap->second.getClientMap().find(*it);
					// check if the user to kick is connected in the channel
					if (posInClientMap == posInChannelMap->second.getClientMap().end())
						message += ERR_USERNOTINCHANNEL(client.getServerName(), client.getNickname(), *it, posInChannelMap->first);
					else
					{
						std::cout << RED << "The print debugger was here: " << RESET << "Kick.cpp:167" << std::endl;
						std::cout	<< BLUE << "Size of client map before erasing: "
									<< RESET << posInChannelMap->second.getClientMap().size() << std::endl;
						posInChannelMap->second.broadcastNumericReply(KICK_MSG(client.getNickname(), \
																		posInChannelMap->first,\
																		posInClientMap->first,\
																		_kickReason));
						posInChannelMap->second.removeConnectedClient(*it);
						std::cout	<< BLUE << "Size of client map after erasing: "
									<< RESET << posInChannelMap->second.getClientMap().size() << std::endl;
					}
				}
			}
		}
	}
	(void)client;
	return message;
}

