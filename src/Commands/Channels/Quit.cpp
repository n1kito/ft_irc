/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:39:27 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:39:28 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Quit.hpp"

/* CONSTRUCTORS ***************************************************************/

Quit::Quit() : ACommand() {}
Quit::Quit(std::map< int, Client >* clients) : ACommand(clients) {}
Quit::Quit(ACommand::clientMap* clients, Quit::channelMap* channels) :
	ACommand(clients),
	_channelMap(channels) {}

Quit::Quit(const Quit &copyMe) : ACommand() { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Quit::~Quit() {}

/* OVERLOADS ******************************************************************/

Quit& Quit::operator = (const Quit &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Quit::action() {}
void		Quit::action(Client& client, std::string& arg)
{
	(void)client;
	(void)arg;
}

void		Quit::parseArgument() {}
void		Quit::parseArgument(Client& client, std::string& arg)
{
	(void)client;
	(void)arg;
}

void		Quit::handleRequest(Client &client, std::string arg)
{
	client.quitServer(arg, _channelMap);
	if (close(client.getClientSocket()) == -1)
		throw std::runtime_error("Error when closing fd");
	_clients->erase(client.getClientSocket());
}
