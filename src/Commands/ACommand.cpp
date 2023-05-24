/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:39:34 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:39:35 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

/* CONSTRUCTORS ***************************************************************/

ACommand::ACommand()
{
	// std::cout << "Default constructor called" << std::endl;
}

ACommand::ACommand(std::map<int, Client>* clients) : _clients(clients) {}

ACommand::ACommand(const ACommand &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

ACommand::~ACommand()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

ACommand& ACommand::operator = (const ACommand &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/

void			ACommand::killClient(int fd, std::string prevMsg, std::string errorMsg)
{
	(void)errorMsg;
	std::string msg;
	msg = prevMsg + KILL(_clients->at(fd).getServerName(), _clients->at(fd).getNickname(), errorMsg.c_str());
	sendNumericReplies(1, _clients->at(fd).getClientSocket(), msg.c_str());

	// usleep(1000);
	if( close( fd ) == -1 )
		throw std::runtime_error("Error when closing fd");
	_clients->erase( fd );
}

Client*			ACommand::getClientByNickname(const std::string& nickname)
{
	for (ACommand::clientMap::iterator it = _clients->begin(); it != _clients->end(); ++it)
		if ((*it).second.getNickname() == nickname)
			return &(*it).second;
	return NULL;
}

// // Trims leading and trailing spaced off of a string/request, as well as any duplicate spaces in the middle
// void		ACommand::cleanRequestSpaces(std::string& request)
// {
// 	// remove leading whitespaces
// 	while (!request.empty() && request.find_first_of(' ') == 0)
// 		request.erase(0, 1);
// 	// and remove them at the end of the request as well
// 	while (!request.empty() && request.find_last_of(' ') == request.size() - 1)
// 		request.erase(request.size() - 1, 1);
// 	for (std::string::iterator it = request.begin(); it != request.end(); ++i)
// 	{
// 		size_t firstSpace = request.find(" ");
// 		if (firstSpace != std::string::npos)
// 		{
// 			while (it != request.end() && )
// 		}
// 	}
// }