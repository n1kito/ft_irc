#include "Kick.hpp"

/* CONSTRUCTORS ***************************************************************/

Kick::Kick() : ACommand() {}

Kick::Kick(std::map< int, Client >* clients) : ACommand(clients) {}

Kick::Kick(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
	ACommand(clients),
	_channelMap(channels)
{}

Kick::Kick(const Kick &copyMe) : ACommand(copyMe) { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Kick::~Kick() {}

/* OVERLOADS ******************************************************************/

Kick& 		Kick::operator = (const Kick &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Kick::parseArgument() {}
void		Kick::action() {}

void		Kick::handleRequest(Client &client, std::string arg)
{
	std::string message = "";

	if (arg.empty())
		message = ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "KICK");
	else
	{
		std::string parseResult = parseArgument(client, arg);
		if (!parseResult.empty())
			message = parseResult;
		else
			message = action(client);
	}
	sendNumericReplies(1, client.getClientSocket(), message.c_str());

	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	std::fill(_userList.begin(), _userList.end(), "");
	_userList.clear();
	_kickReason = "";
}

std::string	Kick::parseArgument(Client &client, std::string& arg)
{
	std::stringstream	argStream(arg);
	std::string			channels	=	"";
	std::string			users		=	"";
	std::string			buffer		=	"";

	std::getline(argStream, channels, ' ');
	std::getline(argStream, users, ' ');
	std::getline(argStream, _kickReason);

	if (users.empty())
		return ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "KICK");
	if (!_kickReason.empty() && _kickReason.at(0) == ':')
		_kickReason.erase(0, 1);
	if (_kickReason.length() > KICKLEN)
		_kickReason = _kickReason.substr(0, KICKLEN);

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

	return "";
}

std::string Kick::action(Client &client)
{
	std::string	message = "";

	for (std::vector< std::string >::iterator chanIt = _channelList.begin(); chanIt != _channelList.end(); ++chanIt)
	{
		channelMap::iterator posInChannelMap	= _channelMap->find(*chanIt);
		if (posInChannelMap == _channelMap->end())
			message += ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), *chanIt);
		else
		{
			Channel* channel = &posInChannelMap->second;
			if (!channel->isClientConnected(client))
				message += ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), *chanIt);
			else if (!channel->isClientOperator(client))
				message += ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), *chanIt);
			else
			{
				for (std::vector< std::string >::iterator itUser = _userList.begin(); itUser != _userList.end(); ++itUser)
				{
					std::map< std::string, const Client* >::const_iterator	userTargetted;
					userTargetted =  channel->getClientMap().find(*itUser);
					if (userTargetted == channel->getClientMap().end())
						message += ERR_USERNOTINCHANNEL(client.getServerName(), client.getNickname(), *itUser, channel->getName());
					else
					{
						channel->broadcastNumericReply(KICK_MSG(client.getNickname(),\
																		client.getUsername(),\
																		channel->getName(),\
																		userTargetted->first,\
																		_kickReason));
						if (channel->isClientOperator(*userTargetted->second))
							channel->removeOperator(*itUser);
						channel->removeConnectedClient(*itUser);

						if (channel->getClientMap().empty())
						{
							client.removeChannel(channel->getName());
							_channelMap->erase(posInChannelMap);
						}
					}
				}
			}
		}
	}
	return message;
}

