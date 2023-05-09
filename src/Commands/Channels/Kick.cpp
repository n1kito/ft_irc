/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 17:02:15 by mjallada          #+#    #+#             */
/*   Updated: 2023/05/09 17:02:15 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kick.hpp"

/* CONSTRUCTORS ***************************************************************/

Kick::Kick() : ACommand() {}

Kick::Kick(std::map< int, Client >* clients) : ACommand(clients) {}


Kick::Kick(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
	ACommand(clients),
	_channels(channels)
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

}

std::string	Kick::parseArgument(Client &client, std::string& arg)
{
	(void)arg;
	(void)client;
	return "";
}

std::string Kick::action(Client &client)
{

	(void)client;
	return "";
}

