#include "Kick.hpp"

/* CONSTRUCTORS ***************************************************************/

Kick::Kick() : ACommand() {}

Kick::Kick(std::map< int, Client >* clients) : ACommand(clients) {}


Kick::Kick(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
	ACommand(clients),
	_channelMap(channels)
{
}

Kick::Kick(const Kick &copyMe) : ACommand(copyMe)
{
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Kick::~Kick()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Kick& Kick::operator = (const Kick &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Kick::parseArgument() {}
void		Kick::action() {}

void	Kick::handleRequest(Client &client, std::string arg)
{
	std::cout << GREEN << "[KICK - handleRequest]\n" << RESET;

	std::string message = "";

	if (arg.empty())
		message = ERR_NEEDMOREPARAMS(client.getServerName(), "KICK");
	else
	{
		std::string parseResult = parseArgument(client, arg);
		if (!parseResult.empty())
			message = parseResult;
		else
			message = action(client);
	}
	send(client.getClientSocket(), message.c_str(), message.length(), 0);

	// cleaning
	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	std::fill(_userList.begin(), _userList.end(), "");
	_userList.clear();
}

std::string	Kick::parseArgument(Client &client, std::string& arg)
{
	std::cout << GREEN << "[KICK - parseArgument]\n" << RESET;

	std::stringstream	argStream(arg);
	std::string			channels	=	"";
	std::string			users		=	"";
	std::string			buffer		=	"";


	std::getline(argStream, channels, ' ');
	std::getline(argStream, users, ' ');
	std::getline(argStream, _kickReason);

	if (users.empty())
		return ERR_NEEDMOREPARAMS(client.getServerName(), "KICK");

	if (!_kickReason.empty() && _kickReason.at(0) == ':') // can it throw an exception  ?
		_kickReason.erase(0, 1);
	
	// init vector of channels
	std::stringstream	channelsStream(channels);
	while (std::getline(channelsStream, buffer, ','))
	{
		leftTrim(buffer, " \r\t\n");
		if (!buffer.empty())
			_channelList.push_back(buffer);
	}

	// init vector of users
	std::stringstream	usersStream(users);
	while (std::getline(usersStream, buffer, ','))
	{
		leftTrim(buffer, " \r\t\n");
		if (!buffer.empty())
			_userList.push_back(buffer);
	}

	// DEBUG mode on **********************************************************
	std::cout		<< RED << "Channels list: " << RESET;
	for (std::vector< std::string >::iterator it = _channelList.begin(); it != _channelList.end(); ++it)
		std::cout	<< *it << " ";
	std::cout		<< std::endl;

	std::cout		<< RED << "Users list: " << RESET;
	for (std::vector< std::string >::iterator it = _userList.begin(); it != _userList.end(); ++it)
		std::cout	<< *it << " ";
	std::cout		<< std::endl;

	std::cout 		<< RED << "Kick reason: " << RESET << _kickReason << std::endl;
	// ************************************************************************

	return "";
}

std::string Kick::action(Client &client)
{
	std::cout << GREEN << "[KICK - action]\n" << RESET;

	std::string	message = "";

	for (std::vector< std::string >::iterator chanIt = _channelList.begin(); chanIt != _channelList.end(); ++chanIt)
	{
		// check if the channel exists and if it does keep its pos in the channel map
		channelMap::iterator posInChannelMap	= _channelMap->find(*chanIt);
		if (posInChannelMap == _channelMap->end())
			message += ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), *chanIt);
		else
		{
			Channel* channel = &posInChannelMap->second;
			// get the client map of the channel found and check if the client that wants to kick someone exists
			if (!channel->isClientConnected(client))
				message += ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), *chanIt);
			// else if (!channel->isClientOperator(client))
			// 	message += ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), *chanIt);
			else
			{
				std::cout << RED << "The kicker is on the channel" << RESET << std::endl;
				for (std::vector< std::string >::iterator itUser = _userList.begin(); itUser != _userList.end(); ++itUser)
				{
					// get the client information of the user to kick
					std::map< std::string, const Client* >::const_iterator	userTargetted;
					userTargetted =  channel->getClientMap().find(*itUser);
					// check if the user to kick is connected in the channel
					if (userTargetted == channel->getClientMap().end())
						message += ERR_USERNOTINCHANNEL(client.getServerName(), client.getNickname(), *itUser, channel->getName());
					else
					{
						channel->broadcastNumericReply(KICK_MSG(client.getServerName(),\
																		client.getNickname(),\
																		channel->getName(),\
																		userTargetted->first,\
																		_kickReason));
						// igit fetch origin
git checkout cg_command-privmsgf kickee was operator => remove from operator
						if (channel->isClientOperator(*userTargetted->second))
							channel->removeOperator(*itUser);
						channel->removeConnectedClient(*itUser);

						// if there is no client in the channel anymore, delete it
						if (channel->getClientMap().empty())
							_channelMap->erase(posInChannelMap);
					}
				}
			}
		}
	}
	std::cout << RED << message << RESET << std::endl;
	return message;
}

