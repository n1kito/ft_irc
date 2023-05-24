/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:39:23 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:39:24 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Part.hpp"

/* CONSTRUCTORS ***************************************************************/

Part::Part() : ACommand() {}

Part::Part(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
	ACommand(clients),
	_channels(channels),
	_reason("")
{
	
}

Part::Part(const Part &copyMe) : ACommand(copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Part::~Part()
{
	// std::cout << "Destructor  called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Part& Part::operator = (const Part &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/
void	Part::parseArgument() {}
void	Part::action() {}
void	Part::action(Client &client)
{
	// check is reason message is valid
	if ( !_reason.empty() && _reason[0] != ':')
	{
		sendNumericReplies(1, client.getClientSocket(), (ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), _reason)).c_str());
		return;
	}
	// erase ':' to display message
	std::size_t colonMarkFinder = _reason.find(':');
	if (colonMarkFinder != std::string::npos)
		_reason.erase(colonMarkFinder, colonMarkFinder + 1);
	
	std::map< std::string, Channel >::iterator it;
	for (size_t i = 0; i < _channelList.size(); i++)
	{
		it = _channels->find(_channelList[i]);
		// return error if channel does not exist
		if (it == _channels->end())
			sendNumericReplies(1, client.getClientSocket(), (ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), _channelList[i])).c_str());
		// return error if channel exist but client is not connected to.
		else if (it != _channels->end() && it->second.getClientMap().find(client.getNickname()) == it->second.getClientMap().end())
			sendNumericReplies(1, client.getClientSocket(), (ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), _channelList[i])).c_str());
		else
		{
			it->second.broadcastNumericReply(PART_MSG(client.getServerName(), client.getNickname(), it->first, _reason));
			it->second.removeInvitedClient(client.getNickname());
			client.removeChannel(_channelList[i]);
		}
	}
}

void Part::parseArgument(std::string& arg)
{
	// std::cout << BLUE << "\n[PART - parseArgument()]\n" << RESET;
	// split the channel list and the key list with a space
	std::stringstream argStream(arg);
	std::string channelArg;
	// std::string message;
	std::getline(argStream, channelArg, ' ');
	std::getline(argStream, this->_reason);

	// split channelArg with ',' and check if valid 
	std::string buffer;
	std::stringstream channelStream(channelArg);
	while (std::getline(channelStream, buffer, ','))
		_channelList.push_back(buffer);

	// /* DEBUG */
	// for (size_t i = 0; i < _channelList.size(); i++)
	// 	std::cout << YELLOW << "<" << _channelList[i] << "> ";
	// std::cout << RESET << "\n";
}

void	Part::handleRequest(Client &client, std::string arg)
{
	// std::cout << BLUE << "\n[PART - handleRequest()]\n" << RESET;
	if (arg.empty())
		sendNumericReplies(1, client.getClientSocket(), (ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "PART")).c_str());
	parseArgument(arg);
	action(client);
	// clear data for next JOIN command 
	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	_reason.clear();
}


