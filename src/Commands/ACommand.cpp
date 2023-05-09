#include "ACommand.hpp"

/* CONSTRUCTORS ***************************************************************/

ACommand::ACommand()
{
	// std::cout << "Default constructor called" << std::endl;
}

ACommand::ACommand(std::map<int, Client>* clients) : _clients(clients) {}

ACommand::ACommand(const ACommand &copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

ACommand::~ACommand()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

ACommand& ACommand::operator = (const ACommand &copyMe)
{
	(void)copyMe;
	// std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

/* METHODS ********************************************************************/

void			ACommand::killClient(int fd, std::string prevMsg, std::string errorMsg)
{
	(void)errorMsg;
	std::string msg;
	msg = prevMsg + KILL(_clients->at(fd).getServerName(), _clients->at(fd).getNickname(), errorMsg.c_str());
	send(_clients->at(fd).getClientSocket(), msg.c_str(), msg.length(), 0);
	// usleep(1000);
	if( close( fd ) == -1 )
		throw std::runtime_error("Error when closing fd");
	_clients->erase( fd );
}

Client*			ACommand::getClientByNickname(const std::string& nickname)
{
	for (ACommand::clientMap::iterator it = _clients->begin(); it != _clients->end(); ++it)
		if ((*it).second.getNickname() == nickname)
			return &(*it).second;
	return NULL;
}
