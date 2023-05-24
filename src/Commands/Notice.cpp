/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:39:39 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:39:40 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Notice.hpp"

/* CONSTRUCTORS ***************************************************************/

Notice::Notice() : ACommand() {}

Notice::Notice(std::map< int, Client >* clients) : ACommand(clients) {}
Notice::Notice(ACommand::clientMap* clients, Notice::channelMap* channels) : ACommand(clients), _channelMap(channels) {}

Notice::Notice(const Notice &copyMe) : ACommand(copyMe) { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Notice::~Notice() {}

/* OVERLOADS ******************************************************************/

Notice& Notice::operator = (const Notice &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Notice::parseArgument() {}
void		Notice::action() {}

void	Notice::handleRequest(Client &client, std::string arg)
{
	if (!arg.empty())
	{
		std::string parseResult = parseArgument(arg);
		if (parseResult.empty())
			action(client);
	}
	std::fill(_targetVector.begin(), _targetVector.end(), "");
	_targetVector.clear();
	_message = "";
}

std::string	Notice::parseArgument(std::string& arg)
{
	std::stringstream	argStream(arg);
	std::string			targets		=	"";
	std::string			buffer		=	"";

	std::getline(argStream, targets, ' ');
	std::getline(argStream, _message);

	if (targets.empty())
		return "ERROR";
	if (_message.empty())
		return "ERROR";
	if (!_message.empty() && _message.at(0) == ':')
		_message.erase(0, 1);
	std::stringstream	targetsStream(targets);
	while (std::getline(targetsStream, buffer, ','))
	{
		leftTrim(buffer, " \r\t\n");
		if (!buffer.empty())
			_targetVector.push_back(buffer);
	}
	return "";
}

void Notice::action(Client &client)
{
	for (std::vector< std::string >::iterator targetIt = _targetVector.begin(); targetIt != _targetVector.end(); ++targetIt)
	{
		if (!targetIt->empty() && targetIt->at(0) == '#')
		{
			channelMap::iterator posInChannelMap	= _channelMap->find(*targetIt);
			if (targetIt->at(1) == '%')
			{
				std::string channelName = *targetIt;
				channelName.erase(1,1);
				posInChannelMap = _channelMap->find(channelName);
			}
			if (posInChannelMap == _channelMap->end())
				return;
			sendToChannel(client, posInChannelMap->second, *targetIt);
		}
		else
		{
			Client* targetClient = getClientByNickname(*targetIt);
			if (!targetClient)
				return;
			sendNumericReplies(1, targetClient->getClientSocket(),\
								NOTICE(client.getNickname(),\
										client.getUsername(), \
										targetClient->getNickname(),\
										_message).c_str());
		}
	}
}

void			Notice::sendToChannel(Client& client, Channel& channel, std::string& target)
{
	// check if the user is on the channel (we didn't implement the +n mode so if the user is not on channel
	// we send ERR_NOTONCHANNEL)
	if (!channel.isClientConnected(client))
		return;
	if (target.size() > 1 && target.at(1) == '%')
	{
		channel.sendMessageToOperators(NOTICE(client.getNickname(),\
												client.getUsername(),\
												channel.getName(),\
												_message),\
												client);
	}
	else
		channel.sendMessageToChannel(NOTICE(client.getNickname(), client.getUsername(), channel.getName(), _message), client);
	return;
}
