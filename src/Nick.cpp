/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeepark <jeepark@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:13:17 by jeepark           #+#    #+#             */
/*   Updated: 2023/04/24 10:35:51 by jeepark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"
#include <vector>
#include <string>

/* CONSTRUCTORS ***************************************************************/

Nick::Nick() { std::cout << "command NICK created.\n"; }

Nick::Nick(const Nick &copyMe) : ACommand(copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	(void)copyMe;
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

const char	*Nick::parseArgument(std::string arg)
{
	(void)arg;
	std::stringstream			s(arg);
	std::string					word;	
	std::vector<std::string>	token;

	// tokenize argument 
	while (s >> word)
		token.push_back(word);

	std::cout << "token size: " << token.size() << "\n";
	// if token is empty, send err_nonicknamegiven
	const char*	message;
	if (token.size() == 0)
		message = ERR_NONICKNAMEGIVEN;
	
	std::cout << message << std::endl;

	return message;
}

const char	*Nick::action(Client &client, std::string nickname)
{
	(void)client;
	(void)nickname;
	return NULL;
}

const char	*Nick::handleRequest(Client &client, std::string arg)
{
	(void)client;
	(void)arg;
	
	parseArgument(arg);

	return NULL;
}

/*
	question : le nombre d'arguments est-il deja parse
	RFC documentation: 
	nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
	- nickname begins with letter or special character
	- nickname len <= 9 
	- authorized characters : letter/digit/[]
	
	ModernIRC doc:
	Nicknames are non-empty strings with the following restrictions:

	They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
	They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
	They MUST NOT start with a character listed as a channel type prefix.
	They SHOULD NOT contain any dot character ('.', 0x2E).
*/

