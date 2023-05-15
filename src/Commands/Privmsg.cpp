#include "Privmsg.hpp"

/* CONSTRUCTORS ***************************************************************/

Privmsg::Privmsg() : ACommand() {}

Privmsg::Privmsg(std::map< int, Client >* clients) : ACommand(clients) {}
Privmsg::Privmsg(ACommand::clientMap* clients, Privmsg::channelMap* channels) : ACommand(clients), _channelMap(channels) {}


Privmsg::Privmsg(const Privmsg &copyMe) : ACommand(copyMe)
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
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Privmsg::parseArgument() {}
void		Privmsg::action() {}


void	Privmsg::handleRequest(Client &client, std::string arg)
{
	std::cout << GREEN << "[PRIVMSG - handleRequest]\n" << RESET;

	std::string message = "";

	if (arg.empty())
		message = ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "PRIVMSG");
	else
	{
		std::string parseResult = parseArgument(client, arg);
		if (!parseResult.empty())
			message = parseResult;
		// else
		// 	message = action(client);
	}
	send(client.getClientSocket(), message.c_str(), message.length(), 0);

	// cleaning
	std::fill(_targetVector.begin(), _targetVector.end(), "");
	_targetVector.clear();
	_message = "";
	
}

std::string	Privmsg::parseArgument(Client &client, std::string& arg)
{
	std::cout << GREEN << "[PRIVMSG - parseArgument]\n" << RESET;

	std::stringstream	argStream(arg);
	std::string			targets		=	"";
	std::string			buffer		=	"";

	std::getline(argStream, targets, ' ');
	std::getline(argStream, _message, ' ');

	if (targets.empty())
		return ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "PRIVMSG");

	// if (!_kickReason.empty() && _kickReason.at(0) == ':') // can it throw an exception  ?
	// 	_kickReason.erase(0, 1);
	
	// init vector of targets
	std::stringstream	targetsStream(targets);
	while (std::getline(targetsStream, buffer, ','))
	{
		leftTrim(buffer, " \r\t\n");
		if (!buffer.empty())
			_targetVector.push_back(buffer);
	}

	// DEBUG mode on **********************************************************
	std::cout		<< RED << "Target list: " << RESET;
	for (std::vector< std::string >::iterator it = _targetVector.begin(); it != _targetVector.end(); ++it)
		std::cout	<< *it << " ";
	std::cout		<< std::endl;

	std::cout 		<< RED << "Message: " << RESET << _message << std::endl;
	// ************************************************************************

	return "";
}

std::string Privmsg::action(Client &client)
{
	(void)_channelMap;
	(void)client;
	return "";
}


// if server sends msg and msg starts with $ ==> sent the message to all clients
// if #%channel => sends message to operators of the chan named channel

//exemple de numeric reply : :sender PRIVMSG receiver :Hello are you receiving this message ?
// autre exempe :sender!serverName@localhost PRIVMSG receiver :Hi everyone!
