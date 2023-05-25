/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:39:32 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:39:33 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Who.hpp"

/* CONSTRUCTORS ***************************************************************/

Who::Who() {}
Who::Who(clientMap* clients) : ACommand(clients), _channelMap(NULL) {}
Who::Who(clientMap* clients, channelMap* channels) :
	ACommand(clients),
	_channelMap(channels) {}

Who::Who(const Who &copyMe) : ACommand() { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Who::~Who() {}

/* OVERLOADS ******************************************************************/

Who& Who::operator = (const Who &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void	Who::parseArgument() {}
void	Who::parseArgument(std::string& arg, std::string& target)
{	
	std::stringstream	argStream(arg);
	argStream >> target;
}

void	Who::handleRequest(Client &client, std::string arg)
{
	std::string	target;
	parseArgument(arg, target);
	action (client, target);
}

void	Who::action() {}
void	Who::action(const Client& client, const std::string& target)
{
	if (target.empty() == false)
	{
		if (target[0] == '#')
		{
			if (_channelMap->find(target) != _channelMap->end())
			{
				Channel* targetChannel = &(_channelMap->at(target));
				const Channel::clientNickMap* targetClientMap = &targetChannel->getClientMap();
				for (Channel::clientNickMap::const_iterator it = targetClientMap->begin(); it != targetClientMap->end(); ++it)
				{			
					const Client* targetClient = it->second;
					sendNumericReplies(1, client.getClientSocket(),
						RPL_WHOREPLY(targetClient->getServerName(), targetClient->getNickname(), target, "~" + targetClient->getUsername(), "localhost", targetClient->getNickname(), "H" + (targetChannel->isClientOperator(*targetClient) ? "@" : ""), targetClient->getRealname()).c_str());
				}
			}
		}
		else
		{
			sendNumericReplies(1, client.getClientSocket(),
				RPL_WHOREPLY(client.getServerName(), client.getNickname(), "*", "~" + client.getUsername(), client.getServerName(), client.getNickname(), "H", client.getRealname()).c_str());
		}
	}
	sendNumericReplies(1, client.getClientSocket(),
		RPL_ENDOFWHO(client.getServerName(), client.getNickname(), target).c_str());
}