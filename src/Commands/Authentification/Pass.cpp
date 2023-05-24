/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:42:42 by mjallada          #+#    #+#             */
/*   Updated: 2023/05/24 13:39:05 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pass.hpp"

/* CONSTRUCTORS ***************************************************************/

Pass::Pass() : ACommand() {}

Pass::Pass(clientMap* clients) : ACommand(clients) {}

Pass::Pass(clientMap* clients, const std::string& serverPassword) :
	ACommand(clients), 
	_serverPassword(serverPassword)
{}

Pass::Pass(const Pass &copyMe) : ACommand()
{
	*this = copyMe;
}

void	Pass::parseArgument(){}
void	Pass::action(){}

void	Pass::handleRequest(Client& client, std::string argument)
{
	std::string message = "";
	if (client.getRegisterState() == true)
		message = ERR_ALREADYREGISTERED(client.getServerName(), client.getNickname());
	else if (argument.length() == 0)
		message = ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "PASS");
	else if (argument != _serverPassword)
	{
		message = ERR_PASSWDMISMATCH(client.getServerName(), client.getNickname());
		// message = ERR_PASSWDMISMATCH(client.getServerName(), client.getNickname());
		killClient(client.getClientSocket(), message, "pass failed");
		return ;
	}
	else
	{
		client.setPassword(argument);
		client.setPasswordStatus(true);
	}
	if (!message.empty())
		sendNumericReplies(1, client.getClientSocket(), message.c_str());
}

/* DESTRUCTORS ****************************************************************/

Pass::~Pass()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Pass& Pass::operator = (const Pass &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/



