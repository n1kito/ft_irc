#include "Join.hpp"

/* CONSTRUCTORS ***************************************************************/

Join::Join() : ACommand() {}

Join::Join(std::map< int, Client >* clients) : ACommand(clients) {}


Join::Join(std::map<int, Client>* clients, std::map< std::string, Channel >* channels) :
	ACommand(clients),
	_channels(channels)
{}

Join::Join(const Join &copyMe) : ACommand(copyMe)
{
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Join::~Join()
{}

/* OVERLOADS ******************************************************************/

Join& Join::operator = (const Join &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/
void		Join::parseArgument() {}
void		Join::action() {}

void 	Join::createErrorTooManyChannels(Client const& client, size_t idx)
{
	std::string message;
	while (idx < _channelList.size())
	{
		message += (ERR_TOOMANYCHANNELS(client.getServerName(), client.getNickname(), _channelList[idx]) + "\r\n");
		idx++;
	}
	sendNumericReplies(1, client.getClientSocket(), message.c_tstr);
	return;
}

std::string		Join::action(Client &client)
{
	//if channel does not exist, create channel
	for (size_t i = 0; i < _channelList.size(); i++)
	{
		// if client has joined too many channels (max=20) return error
		if (client.getChannelsMap().size() >= MAXCHANNELS)
		{
			createErrorTooManyChannels(client, i);
			return;
		}
		std::map<std::string, Channel>::iterator it = _channels->find(_channelList[i]);
		// if doesn't exist, create channel (its ctor adds itself client)
		if (it == _channels->end())
		{
			Channel newChannel(_channelList[i], client);
			(*_channels)[_channelList[i]] = newChannel;
			client.addChannel((*_channels)[_channelList[i]]);
		}
		// else add client to existing channel
		else
		{
			// check if channel is already full
			if (it->second.getClientMap().size() >= MAXCLIENTS)
				sendNumericReplies(1, client.getClientSocket(), ERR_CHANNELISFULL(client.getServerName(), client.getNickname(), it->second.getName()));
				// return (ERR_CHANNELISFULL(client.getServerName(), client.getNickname(), it->second.getName()));
			// check if channel has set limit for nb of users 
			if (it->second.modeIs("client-limit") && it->second.getClientMap().size() >= it->second.getClientLimit())
				sendNumericReplies(1, client.getClientSocket(), ERR_CHANNELISFULL(client.getServerName(), client.getNickname(), it->second.getName()));
				// return (ERR_CHANNELISFULL(client.getServerName(), client.getNickname(), it->second.getName()));
			// check if channel has set invite-only mode
			if (it->second.modeIs("invite-only") && !it->second.isInvited(client.getNickname()))
				sendNumericReplies(1, client.getClientSocket(), ERR_INVITEONLYCHAN(client.getServerName(), client.getNickname(), it->second.getName()));
				// return (ERR_INVITEONLYCHAN(client.getServerName(), client.getNickname(), it->second.getName()));
			// check if channel is in protected mode 
			if (it->second.modeIs("key"))
			{
				// if key is incorrect, cannot join channel and send error
                if (_keyList.empty() || (i < _keyList.size() && it->second.getKey() != _keyList[i]))
					sendNumericReplies(1, client.getClientSocket(), ERR_BADCHANNELKEY(client.getServerName(), client.getNickname(), it->second.getName()));
					// return (ERR_BADCHANNELKEY(client.getServerName(), client.getNickname(), it->second.getName()));
			}
			it->second.addConnectedClient(client);
			client.addChannel(it->second);
		}
	}
	return;
}


void	Join::parseArgument(Client& client, std::string& arg)
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
		// check if channel channel name's length is at least 2 (including #)
		if (buffer.size() < 2 || buffer.size() > CHANLEN)
		{
			sendNumericReplies(1, client.getClientSocket(), ERR_BADCHANMASK(client.getServerName(), buffer));
			return;
		}
			// return (ERR_BADCHANMASK(client.getServerName(), buffer));
		else if (buffer[0] != '#')
		{
			sendNumericReplies(1, client.getClientSocket(), ERR_BADCHANAME(client.getServerName(), client.getNickname(), buffer));
			return;
		}
			// return (ERR_BADCHANAME(client.getServerName(), client.getNickname(), buffer));
		else
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
	
	return;
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
		parseArgument(client, arg);
		if (!_keyList.empty())
			action(client);
	}
	// send(client.getClientSocket(), message.c_str(), message.length(), 0);
	// clear data for next JOIN command 
	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	std::fill(_keyList.begin(), _keyList.end(), "");
	_keyList.clear();
}
