#ifndef NICK_HPP
# define NICK_HPP

#include "ACommand.hpp"
#include "ft_irc.hpp"
#include "Client.hpp"
#include <map>

class Nick : public ACommand
{
	public:
		Nick(const std::map<int, Client >* clients);
		~Nick();

    	std::string	handleRequest(Client &client, std::string arg);
    	void		parseArgument();
    	std::string	parseArgument(Client &client, std::string arg);
    	void		action();
    	std::string	action(Client &client, std::string nickname);

		bool		isValidNickname(std::string nickname);
	protected:
		// add protected elements here

	private:
		// add private elements here
    	Nick();
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
	Nicknames are non-empty strings with the following restrictions:

They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
They MUST NOT start with a character listed as a channel type prefix.
They SHOULD NOT contain any dot character ('.', 0x2E).
Servers MAY have additional implementation-specific nickname restrictions.
*/