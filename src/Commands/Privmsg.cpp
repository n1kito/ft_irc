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
		else
			message = action(client);
	}
	if (message.length() > 0)
		sendNumericReplies(1, client.getClientSocket(), message.c_str());

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
	std::getline(argStream, _message);

	if (targets.empty())
		return ERR_NORECIPIENT(client.getServerName(), client.getNickname());
	if (_message.empty())
		return ERR_NOTEXTTOSEND(client.getServerName(), client.getNickname());
	if (!_message.empty() && _message.at(0) == ':') // can it throw an exception  ?
		_message.erase(0, 1);
	
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
	std::cout << GREEN << "[PRIVMSG - action]\n" << RESET;

	std::string	message = "";

	for (std::vector< std::string >::iterator targetIt = _targetVector.begin(); targetIt != _targetVector.end(); ++targetIt)
	{
		// check if it is a channel
		std::cout << RED << "Target name: " << RESET << *targetIt << std::endl;
		if (!targetIt->empty() && targetIt->at(0) == '#')
		{
			channelMap::iterator posInChannelMap	= _channelMap->find(*targetIt);
			if (targetIt->at(1) == '%')
			{
				std::string channelName = *targetIt;
				channelName.erase(1,1);
				posInChannelMap = _channelMap->find(channelName);
			}
			// check if the channel exists and if it does, keep its pos in the channel map
			if (posInChannelMap == _channelMap->end())
				return ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), *targetIt);
			
			message = sendToChannel(client, posInChannelMap->second, *targetIt);
		}
		else
		{
			Client* targetClient = getClientByNickname(*targetIt);
			if (!targetClient)
				return ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), *targetIt);
			
			// sendNumericReplies(1, targetClient->getClientSocket(),\
			// 					PRIVMSG(client.getServerName(),\
			// 							client.getNickname(), \
			// 							targetClient->getNickname(),\
			// 							_message).c_str());
			sendNumericReplies(1, targetClient->getClientSocket(),\
								PRIVMSG(client.getNickname(),\
										client.getUsername(), \
										targetClient->getNickname(),\
										_message).c_str());
			std::cout << RED << "Targetted client :" << RESET << targetClient->getNickname() << std::endl;
			// getClientByNickname
		}

	}
	std::cout << RED << message << RESET << std::endl;
	return message;
}
	// (void)_channelMap;
	// (void)client;

std::string			Privmsg::sendToChannel(Client& client, Channel& channel, std::string& target)
{
	std::string message = "";

	// check if the user is on the channel (we didn't implement the +n mode so if the user is not on channel
	// we send ERR_NOTONCHANNEL)
	if (!channel.isClientConnected(client))
		return ERR_CANNOTSENDTOCHAN(client.getServerName(), client.getNickname(), channel.getName());
	if (target.size() > 1 && target.at(1) == '%')
	{
		channel.sendMessageToOperators(PRIVMSG(client.getNickname(),\
												client.getUsername(),\
												channel.getName(),\
												_message),\
												client);
	}
	else
		channel.sendMessageToChannel(PRIVMSG(client.getNickname(), client.getUsername(), channel.getName(), _message), client);
	return message;
}


// if server sends msg and msg starts with $ ==> sent the message to all clients
// if #%channel => sends message to operators of the chan named channel

//exemple de numeric reply : :sender PRIVMSG receiver :Hello are you receiving this message ?
// autre exempe :sender!serverName@localhost PRIVMSG receiver :Hi everyone!
