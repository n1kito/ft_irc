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

/* CONSTRUCTORS ***************************************************************/

Nick::Nick() {}

Nick::Nick(std::string arg)
{
	
}

Nick::Nick(const Nick &copyMe)
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
	return *this;
}

/* METHODS ********************************************************************/

char	*Nick::parseArgument(std::string arg)
{
	std::stringstream			s;
	std::string					word;	
	std::vector<std::string>	token;

	while (s >> word) { // Extract word from the stream.
		token.push_back(word);
    }
	// if (token.size() != 1)
	// 	return ()
}

char	*Nick::handleRequest(Client &client, std::string arg)
{
	
	parseArgument(arg);
}

/*
	question : le nombre d'arguments est-il deja parse 
*/

