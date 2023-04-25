/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeepark <jeepark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:13:17 by jeepark           #+#    #+#             */
/*   Updated: 2023/04/21 17:57:53 by jeepark          ###   ########.fr       */
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
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Nick::~Nick()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Nick& Nick::operator = (const Nick &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Nick::parseArgument() {}

bool	Nick::isValidNickname(std::string nickname){
	if(nickname.find_first_not_of(".,*?!@ ") != std::string::npos)
		return false;
	// check if nickname starts with any forbidden character
	char firstChar = nickname[0];
	if (firstChar == '$' || firstChar == ':' || firstChar == '#' || firstChar == '&')
		return false;
	return true;
}



std::string	Nick::parseArgument(Client &client, std::string arg)
{
	(void)arg;
	
	std::stringstream			s;
	std::string					word;	
	std::vector<std::string>	token;

	while (s >> word) {
		token.push_back(word);
    }
	// check if token is not empty
	if (token.size() == 0)
		return (ERR_NONICKNAMEGIVEN(arg));
	
	if (token.size() > 1 || !isValidNickname(token[0]))
			return(ERR_ERRONEUSNICKNAME(client.getNickname(), arg));

	// check if nickname already exists



	
	return "";
}

std::string	Nick::handleRequest(Client &client, std::string arg)
{
	std::string message;
	std::cout << "handlerequest nick\n";
	message = parseArgument(client, arg);
	message = action(client, arg);	
	return message;
}

void		Nick::action() {}

std::string	Nick::action(Client &client, std::string nickname)
{
	std::string message;
	client.setNickname(nickname);
	message = NICK_SUCCESS("server", client.getNickname());

	return message;
}

/*
	question : le nombre d'arguments est-il deja parse 
*/

