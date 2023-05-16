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

bool	Nick::isValidNickname(std::string nickname)
{
	// check if nickname is no longer than 10 
	if (nickname.size() > NICKLEN)
		return false;
	// check if nickname contains any forbbiden character
	if(nickname.find_first_of(".,*?!@ ") != std::string::npos)
	{
		// std::cout << "forbidden character\n";
		return false;
	}
	// check if nickname starts with any forbidden character
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#' || nickname[0] == '&')
		return false;
	return true;
}

void		Nick::parseArgument() {}


std::string	Nick::parseArgument(Client &client, std::string& arg)
{
	// check if arg is empty
	if (arg.empty())
		return (ERR_NONICKNAMEGIVEN(client.getServerName()));
	// check if nickname format is valid
	if (!isValidNickname(arg))
		return(ERR_ERRONEUSNICKNAME(client.getServerName(), "nickname", arg));

	// check if nickname already exists
	std::map<int, Client>::const_iterator it = _clients->begin();
	std::map<int, Client>::const_iterator ite = _clients->end();

	while(it != ite)
	{
		if (it->second.getNickname() == arg && it->second.getClientSocket() != client.getClientSocket() )
		{
			std::string msg; 
			if (!client.getWelcomeState())
			{
				msg = ERR_NICKCOLLISION(client.getServerName(), arg);
				killClient(client.getClientSocket(), msg, "nickname collision failed" );
				return("Client has been killed");
			}
			msg = ERR_NICKNAMEINUSE(client.getServerName(), arg);
			return(msg);
		}
		it++;
	}
	return ("Nickname is valid");
}
void		Nick::action() {}
std::string	Nick::action(Client &client, std::string nickname)
{
	std::string message;
	message = NICK_SUCCESS(client.getNickname(), nickname);
	client.setNickname(nickname);
	return message;
}

void	Nick::handleRequest(Client &client, std::string arg)
{
	std::string message = parseArgument(client, arg);
	if (message == "Client has been killed")
		return;
	else if (message == "Nickname is valid")
	{
		message = action(client, arg);
		// std::cout << BLUE << "\n\nHANDLE REQUEST\n" << message << "\n" ;
		send(client.getClientSocket(), message.c_str(), message.length(), 0);
		// std::cout << "nickname is:" << client.getNickname() << "message:" << message << "|\n" << RESET;
		return;

	}
	else
	{
		send(client.getClientSocket(), message.c_str(), message.length(), 0);
	}

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
