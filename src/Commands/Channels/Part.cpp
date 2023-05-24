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
{}

Part::Part(const Part &copyMe) : ACommand(copyMe) { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Part::~Part() {}

/* OVERLOADS ******************************************************************/

Part& Part::operator = (const Part &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/
void	Part::parseArgument() {}
void	Part::action() {}
void	Part::action(Client &client)
{
	if (!_reason.empty() && _reason[0] != ':')
	{
		sendNumericReplies(1, client.getClientSocket(), (ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), _reason)).c_str());
		return;
	}
	std::size_t colonMarkFinder = _reason.find(':');
	if (colonMarkFinder != std::string::npos)
		_reason.erase(colonMarkFinder, colonMarkFinder + 1);
	
	std::map< std::string, Channel >::iterator it;
	for (size_t i = 0; i < _channelList.size(); i++)
	{
		it = _channels->find(_channelList[i]);
		if (it == _channels->end())
			sendNumericReplies(1, client.getClientSocket(), (ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), _channelList[i])).c_str());
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
	std::stringstream argStream(arg);
	std::string channelArg;

	std::getline(argStream, channelArg, ' ');
	std::getline(argStream, this->_reason);

	std::string buffer;
	std::stringstream channelStream(channelArg);
	while (std::getline(channelStream, buffer, ','))
		_channelList.push_back(buffer);
}

void	Part::handleRequest(Client &client, std::string arg)
{
	if (arg.empty())
		sendNumericReplies(1, client.getClientSocket(), (ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "PART")).c_str());
	parseArgument(arg);
	action(client);

	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	_reason.clear();
}