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
void		Nick::parseArgument() {}
void		Nick::action() {}

bool	Nick::isValidNickname(std::string nickname)
{
	// check if nickname is no longer than 10 
	if (nickname.size() > NICKLEN)
		return false;
	// check if nickname contains any forbbiden character
	if(nickname.find_first_of(".,*?!@ ") != std::string::npos)
	{
		return false;
	}
	// check if nickname starts with any forbidden character
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#' || nickname[0] == '&')
		return false;
	return true;
}


std::string	Nick::parseArgument(Client &client, std::string& arg)
{
	// check if arg is empty
	if (arg.empty())
		return (ERR_NONICKNAMEGIVEN(client.getServerName()));
	// check if nickname format is valid
	if (!isValidNickname(arg))
		return(ERR_ERRONEUSNICKNAME(client.getServerName(), "nickname", arg));

	// check if nickname already exists
	std::map<int, Client>::const_iterator ClientIt = _clients->begin();

	while(ClientIt != _clients->end())
	{
		if (ClientIt->second.getNickname() == arg && ClientIt->second.getClientSocket() != client.getClientSocket() )
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
		ClientIt++;
	}
	return ("Nickname is valid");
}

std::string	Nick::action(Client &client, std::string nickname)
{
	std::string message = NICK_SUCCESS(client.getNickname(), nickname);
	client.setNickname(nickname);
	return (message);
}

void	Nick::handleRequest(Client &client, std::string arg)
{
	std::string message = parseArgument(client, arg);
	if (message == "Client has been killed")
		return;
	else if (message == "Nickname is valid")
	{
		message = action(client, arg);
		std::map< std::string, Channel* >::const_iterator chanIt = client.getChannelsMap().begin();
		if (chanIt != client.getChannelsMap().end())
			chanIt->second->broadcastNumericReplies(1, message.c_str());
		else
			sendNumericReplies(1, client.getClientSocket(), message.c_str());
		return;
	}
	else
		sendNumericReplies(1, client.getClientSocket(), message.c_str());
}
