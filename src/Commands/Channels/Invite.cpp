#include "Invite.hpp"

/* CONSTRUCTORS ***************************************************************/

Invite::Invite() {}
Invite::Invite(Invite::clientMap* clients) : ACommand(clients) {}
Invite::Invite(Invite::clientMap* clients, Invite::channelMap* channels) : ACommand(clients), _channelMap(channels) {}

Invite::Invite(const Invite &copyMe) : ACommand() { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Invite::~Invite() { }

/* OVERLOADS ******************************************************************/

Invite& Invite::operator = (const Invite &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void	Invite::parseArgument() {}
void	Invite::parseArgument(std::string arg, std::string& channel, std::string& request)
{	
	
}

void		Invite::handleRequest(Client &client, std::string arg)
{
}

void	Invite::action() { }
void	Invite::action(std::string& topic, Channel& targetChannel, const Client& client)
{

}