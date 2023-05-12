/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeepark <jeepark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 14:09:42 by jeepark           #+#    #+#             */
/*   Updated: 2023/05/12 15:26:03 by jeepark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Part.hpp"

/* CONSTRUCTORS ***************************************************************/

Part::Part() : ACommand() {}

Part::Part(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
	ACommand(clients),
	_channels(channels)
{
	
}

Part::Part(const Part &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Part::~Part()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Part& Part::operator = (const Part &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/
void			Part::parseArgument() {}
void			Part::action() {}
std::string		Part::action(Client &client)
{
	(void)client;
	std::string res;
	std::map< std::string, Channel >::iterator it;
	
	for (size_t i = 0; i < _channelList.size(); i++)
	{
		it = _channels->find(_channelList[i]);
		// return error if channel does not exist
		if (it == _channels->end())
			return ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), _channelList[i]);
		// return error if channel exist but client is not connected to.
		else if (it != _channels->end() && it->second.getClientMap().find(client.getNickname()) == it->second.getClientMap().end())
			return ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), _channelList[i]);
		// res += PART_MSG(client.getServerName(), client.getNickname(), _channelList[i]);
		client.removeChannel(_channelList[i]);
	}
	
	return "";
}

std::string	Part::parseArgument(std::string& arg)
{
	// split the channel list and the key list with a space
	std::stringstream argStream(arg);
	std::string channelArg;
	std::string message;
	std::getline(argStream, channelArg, ' ');
	std::getline(argStream, message);
	
	// split channelArg with ',' and check if valid 
	std::string buffer;
	std::stringstream channelStream(channelArg);
	while (std::getline(channelStream, buffer, ','))
	{
		// check if channel channel name's length is at least 1
		// if (buffer[0] != "#")
		_channelList.push_back(buffer);
	}

	// // split keyArg with ',' 
	// std::stringstream keyStream(keyArg);
	// while (std::getline(keyStream, buffer, ','))
	// 	_keyList.push_back(buffer);

	/* DEBUG */
	for (size_t i = 0; i < _channelList.size(); i++)
		std::cout << YELLOW << _channelList[i] << " ";
	std::cout << RESET << "\n";
	
	return "";
}

void	Part::handleRequest(Client &client, std::string arg)
{
	std::cout << BLUE << "\n[PART - handleRequest()]\n" << RESET;
	std::string message = "";
	if (arg.empty())
		message = ERR_NEEDMOREPARAMS(client.getServerName(), "PART");
	std::string parseResults = parseArgument(arg);
	if (!parseResults.empty())
			message = parseResults;
	else
			message = action(client);
	send(client.getClientSocket(), message.c_str(), message.length(), 0);
	// clear data for next JOIN command 
	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	
	
}


