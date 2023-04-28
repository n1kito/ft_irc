#include "Pass.hpp"

/* CONSTRUCTORS ***************************************************************/

Pass::Pass() : ACommand() {}

Pass::Pass(clientMap* clients) : ACommand(clients) {}

Pass::Pass(clientMap* clients, const std::string& serverPassword) :
	ACommand(clients),
	_serverPassword(serverPassword)
{}

Pass::Pass(const Pass &copyMe) : ACommand()
{
	*this = copyMe;
}

void	Pass::parseArgument(){}
void	Pass::action(){}

std::string	Pass::handleRequest(Client& client, std::string argument)
{
	if (client.getRegisterState() == true)
		return ERR_ALREADYREGISTERED(client.getServerName(), client.getNickname());
	else if (argument.length() == 0)
		return ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "PASS");
	else if (argument != _serverPassword)
	{
		std::cout << "PASSWORDS DO NOT MATCH" << std::endl; // TODO: remove this
		return ERR_PASSWDMISMATCH(client.getServerName(), client.getNickname());
	}
	client.setPassword(argument);
	client.setPasswordStatus(true);
	return PASS_SUCCESS(client.getServerName(), "pouet");
}

/* DESTRUCTORS ****************************************************************/

Pass::~Pass()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Pass& Pass::operator = (const Pass &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/



