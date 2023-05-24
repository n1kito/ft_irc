/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 17:39:23 by jeepark           #+#    #+#             */
/*   Updated: 2023/05/24 13:39:04 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include <vector>
#include <string>
#include <map>
#include "Client.hpp"
#include "numericReplies.hpp"

/* CONSTRUCTORS ***************************************************************/

Nick::Nick() {}

Nick::Nick(std::map<int, Client>* clients) : ACommand(clients) {}

Nick::Nick(const Nick &copyMe) : ACommand()
{ *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Nick::~Nick() {}

/* OVERLOADS ******************************************************************/

Nick& 		Nick::operator = (const Nick &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/
void		Nick::parseArgument() {}
void		Nick::action() {}

bool		Nick::isValidNickname(std::string nickname)
{
	if (nickname.size() > NICKLEN)
		return false;
	if(nickname.find_first_of(".,*?!@ ") != std::string::npos)
		return false;
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#' || nickname[0] == '&')
		return false;
	return true;
}

std::string	Nick::parseArgument(Client &client, std::string& arg)
{
	if (arg.empty())
		return (ERR_NONICKNAMEGIVEN(client.getServerName()));
	if (!isValidNickname(arg))
		return(ERR_ERRONEUSNICKNAME(client.getServerName(), "nickname", arg));
	std::map<int, Client>::const_iterator ClientIt = _clients->begin();
	while(ClientIt != _clients->end())
	{
		if (ClientIt->second.getNickname() == arg && ClientIt->second.getClientSocket() != client.getClientSocket())
		{
			std::string msg; 
			if (!client.getWelcomeState())
			{
				msg = ERR_NICKCOLLISION(client.getServerName(), arg);
				killClient(client.getClientSocket(), msg, "nickname collision failed");
				return("Client has been killed");
			}
			msg = ERR_NICKNAMEINUSE(client.getServerName(), arg);
			return(msg);
		}
		ClientIt++;
	}
	return ("Nickname is valid");
}

std::string	Nick::action(Client &client, std::string nickname)
{
	std::string message = NICK_SUCCESS(client.getNickname(), nickname);
	client.setNickname(nickname);
	return (message);
}

void	Nick::handleRequest(Client &client, std::string arg)
{
	std::string message = parseArgument(client, arg);
	if (message == "Client has been killed")
		return;
	else if (message == "Nickname is valid")
	{
		message = action(client, arg);
		std::map< std::string, Channel* >::const_iterator chanIt = client.getChannelsMap().begin();
		if (chanIt != client.getChannelsMap().end())
			chanIt->second->broadcastNumericReply(message.c_str());
		else
			sendNumericReplies(1, client.getClientSocket(), message.c_str());
		return;
	}
	else
		sendNumericReplies(1, client.getClientSocket(), message.c_str());
}
