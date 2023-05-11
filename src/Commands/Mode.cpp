#include "Mode.hpp"

/* CONSTRUCTORS ***************************************************************/

Mode::Mode() {}
Mode::Mode(clientMap* clients) : ACommand(clients), _channelMap(NULL) {}
Mode::Mode(clientMap* clients, channelMap* channels) : ACommand(clients), _channelMap(channels) {}

Mode::Mode(const Mode &copyMe) : ACommand()
{
	// std::cout << "Copy constructor called" << std::endl;
	(void)copyMe;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Mode::~Mode()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Mode& Mode::operator = (const Mode &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Mode::parseArgument() {}
// std::string	Mode::parseArgument(Client &client, std::string& arg)
// {
// }

void		Mode::action() {}
// std::string	Mode::action(Client &client, std::string nickname)
// {
// 	std::string message;
// 	message = NICK_SUCCESS(client.getModename(), nickname);
// 	client.setModename(nickname);
// 	return message;
// }

void	Mode::handleRequest(Client &client, std::string arg)
{
	(void)client;
	(void)arg;
	(void)_channelMap;
	std::cout << arg << std::endl;
}