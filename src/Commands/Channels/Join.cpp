#include "Join.hpp"

/* CONSTRUCTORS ***************************************************************/

Join::Join() : ACommand() {}

Join::Join(std::map< int, Client >* clients) : ACommand(clients) {}


Join::Join(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
	ACommand(clients),
	_channels(channels)
{
	
}

Join::Join(const Join &copyMe) : ACommand(copyMe)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Join::~Join()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Join& Join::operator = (const Join &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/
void		Join::parseArgument() {}
void		Join::action() {}

std::string 	Join::createErrorTooManyChannels(Client const& client, size_t idx)
{
	std::string message;
	while (idx < _channelList.size())
	{
		message += (ERR_TOOMANYCHANNELS(client.getServerName(), client.getNickname(), _channelList[idx]) + "\r\n");
		idx++;
	}
	return message;
}

std::string		Join::action(Client &client)
{
	//if channel does not exist, create channel
	for (size_t i = 0; i < _channelList.size(); i++)
	{
		// if client has joined too many channels (max=20) return error
		if (client.getChannelsMap().size() >= MAXCHANNELS)
			return (createErrorTooManyChannels(client, i));
		std::map<std::string, Channel>::iterator it = _channels->find(_channelList[i]);
		// if doesn't exist, create channel (its ctor adds itself client)
		if (it == _channels->end())
		{
			Channel newChannel(_channelList[i], client);
			(*_channels)[_channelList[i]] = newChannel;
			// (*_channels)[_channelList[i]].addOperator(client);
			// if a key is associated to channel, set Protection mode to channel and add key
			// if (i < _keyList.size() && _keyList[i] != "x")
			// {
			// 	(*_channels)[_channelList[i]].setChannelProtection(true);
			// 	if (!_keyList[i].empty())
			// 		(*_channels)[_channelList[i]].setKey(_keyList[i]);
			// }
			client.addChannel((*_channels)[_channelList[i]]);
		}
		// else add client to existing channel
		else
		{
			// TODO: MODE
			// check if channel has set limit for nb users 
			if (it->second.modeIs("client-limit") && it->second.getClientMap().size() >= static_cast< size_t >(std::atoi(it->second.getModeParameter('l').c_str())))
				return (ERR_CHANNELISFULL(client.getServerName(), client.getNickname(), it->second.getName()));
			if (it->second.modeIs("invite-only") && !it->second.isInvited(client.getNickname()))
				return (ERR_INVITEONLYCHAN(client.getServerName(), client.getNickname(), it->second.getName()));
			if (it->second.modeIs("key"))
			// if (!it->second.getKey().empty())
			{
				// if key is incorrect, cannot join channel and send error
                if (_keyList.empty() || (i < _keyList.size() && it->second.getModeParameter('k') != _keyList[i]))
					return (ERR_BADCHANNELKEY(client.getServerName(), client.getNickname(), it->second.getName()));
			}
			it->second.addConnectedClient(client);
			client.addChannel(it->second);
		}
	}
	return "";
}


std::string	Join::parseArgument(std::string& arg)
{
	// split the channel list and the key list with a space
	std::stringstream argStream(arg);
	std::string channelArg;
	std::string keyArg;
	std::getline(argStream, channelArg, ' ');
	std::getline(argStream, keyArg);
	
	// split channelArg with ',' and check if valid 
	std::string buffer;
	std::stringstream channelStream(channelArg);
	while (std::getline(channelStream, buffer, ','))
	{
		// check if channel channel name's length is at least 1
		if (buffer.size() <= 1 || buffer.size() > CHANLEN)
			return (ERR_BADCHANMASK(buffer));
		_channelList.push_back(buffer);
	}

	// split keyArg with ',' 
	std::stringstream keyStream(keyArg);
	while (std::getline(keyStream, buffer, ','))
		_keyList.push_back(buffer);

	/* DEBUG */
	for (size_t i = 0; i < _channelList.size(); i++)
		std::cout << YELLOW << _channelList[i] << " ";
	std::cout << RESET << "\n";
	for (size_t i=0; i < _keyList.size(); i++)
		std::cout << GREEN << _keyList[i] << " ";
	std::cout << RESET << "\n";
	
	return "";
}

void	Join::handleRequest(Client &client, std::string arg)
{
	std::string message = "";
	if (arg.empty())
		message = ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "JOIN");
	else if (arg == "0")
	{
		client.leaveAllChannels();
		return;
	}
	else
	{
		std::string parseResults = parseArgument(arg);
		if (!parseResults.empty())
			message = parseResults;
		else
			message = action(client);
	}
	send(client.getClientSocket(), message.c_str(), message.length(), 0);
	// clear data for next JOIN command 
	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	std::fill(_keyList.begin(), _keyList.end(), "");
	_keyList.clear();
}
