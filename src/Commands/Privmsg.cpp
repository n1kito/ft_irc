#include "Privmsg.hpp"

/* CONSTRUCTORS ***************************************************************/

Privmsg::Privmsg() : ACommand() {}

Privmsg::Privmsg(std::map< int, Client >* clients) : ACommand(clients) {}
Privmsg::Privmsg(ACommand::clientMap* clients, Privmsg::channelMap* channels) : ACommand(clients), _channelMap(channels) {}

Privmsg::Privmsg(const Privmsg &copyMe) : ACommand(copyMe) { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Privmsg::~Privmsg() {}

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

	std::fill(_targetVector.begin(), _targetVector.end(), "");
	_targetVector.clear();
	_message = "";
}

std::string	Privmsg::parseArgument(Client &client, std::string& arg)
{
	std::stringstream	argStream(arg);
	std::string			targets		=	"";
	std::string			buffer		=	"";

	std::getline(argStream, targets, ' ');
	std::getline(argStream, _message);

	if (targets.empty())
		return ERR_NORECIPIENT(client.getServerName(), client.getNickname());
	if (_message.empty())
		return ERR_NOTEXTTOSEND(client.getServerName(), client.getNickname());
	if (!_message.empty() && _message.at(0) == ':')
		_message.erase(0, 1);
	
	std::stringstream	targetsStream(targets);
	while (std::getline(targetsStream, buffer, ','))
	{
		leftTrim(buffer, " \r\t\n");
		if (!buffer.empty())
			_targetVector.push_back(buffer);
	}
	return "";
}

std::string Privmsg::action(Client &client)
{
	std::string	message = "";

	for (std::vector< std::string >::iterator targetIt = _targetVector.begin(); targetIt != _targetVector.end(); ++targetIt)
	{
		if (!targetIt->empty() && targetIt->at(0) == '#')
		{
			channelMap::iterator posInChannelMap = _channelMap->find(*targetIt);
			if (targetIt->at(1) == '%')
			{
				std::string channelName = *targetIt;
				channelName.erase(1,1);
				posInChannelMap = _channelMap->find(channelName);
			}
			if (posInChannelMap == _channelMap->end())
				return ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), *targetIt);
			
			message = sendToChannel(client, posInChannelMap->second, *targetIt);
		}
		else
		{
			Client* targetClient = getClientByNickname(*targetIt);
			if (!targetClient)
				return ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), *targetIt);
			sendNumericReplies(1, targetClient->getClientSocket(),\
								PRIVMSG(client.getNickname(),\
										client.getUsername(), \
										targetClient->getNickname(),\
										_message).c_str());
		}
	}
	return message;
}

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