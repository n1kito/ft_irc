/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeepark <jeepark@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:17:03 by jeepark           #+#    #+#             */
/*   Updated: 2023/05/16 16:18:46 by jeepark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Quit.hpp"

/* CONSTRUCTORS ***************************************************************/

Quit::Quit() : ACommand() {}

Quit::Quit(std::map< int, Client >* clients) : ACommand(clients) {}

// Quit::Quit(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
// 	ACommand(clients),
// 	_channelMap(channels)
// {
// }

Quit::Quit(const Quit &copyMe) : ACommand()
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Quit::~Quit()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Quit& Quit::operator = (const Quit &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/

void		Quit::parseArgument() {}
void		Quit::action() {}

void		Quit::action(Client& client, std::string& arg)
{
	(void)client;
	(void)arg;
	std::cout << GREEN << "[QUIT - Action]\n" << RESET;
	
}
void		Quit::parseArgument(Client& client, std::string& arg)
{
	(void)client;
	(void)arg;
	std::cout << GREEN << "[QUIT - parseArgument]\n" << RESET;

	
}

void	Quit::handleRequest(Client &client, std::string arg)
{
	(void)arg;
	// std::cout << GREEN << "[QUIT - handleRequest]\n" << RESET;
	client.QuitServer(arg);
	if (close( client.getClientSocket() ) == -1)
		throw std::runtime_error("Error when closing fd");
	_clients->erase(client.getClientSocket());
}
