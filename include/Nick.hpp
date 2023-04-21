/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeepark <jeepark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:13:17 by jeepark           #+#    #+#             */
/*   Updated: 2023/04/21 17:30:24 by jeepark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
# define NICK_HPP

#include "ACommand.hpp"
#include "ft_irc.hpp"
#include "Client.hpp"

class Nick
{
	public:
		Nick();
		~Nick();

      char* handleRequest(Client &client, std::string arg);
      char* parseArgument(std::string arg);
      char* action(Client &client, std::string nickname);
      
		
	protected:
		// add protected elements here

	private:
		// add private elements here
		Nick(const Nick &copyMe);
		Nick&		operator = (const Nick &copyMe);

		
};

#endif

/*
      Command: NICK
   Parameters: <nickname>

   NICK command is used to give user a nickname or change the existing
   one.




Kalt                         Informational                     [Page 10]

RFC 2812          Internet Relay Chat: Client Protocol        April 2000


   Numeric Replies:

           ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
           ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
           ERR_UNAVAILRESOURCE             ERR_RESTRICTED

   Examples:

   NICK Wiz                ; Introducing new nick "Wiz" if session is
                           still unregistered, or user changing his
                           nickname to "Wiz"

   :WiZ!jto@tolsun.oulu.fi NICK Kilroy
                           ; Server telling that WiZ changed his
                           nickname to Kilroy.
*/