/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli <cgosseli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:42:11 by cgosseli          #+#    #+#             */
/*   Updated: 2023/05/12 17:43:23 by cgosseli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Privmsg.hpp"

/* CONSTRUCTORS ***************************************************************/

Privmsg::Privmsg()
{
	// std::cout << "Default constructor called" << std::endl;
}

Privmsg::Privmsg(const Privmsg &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Privmsg::~Privmsg()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Privmsg& Privmsg::operator = (const Privmsg &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/



