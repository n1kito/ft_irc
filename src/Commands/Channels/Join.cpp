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

std::string		Join::action(Client &client)
{
	std::cout << BLUE << "[JOIN - action]\n" << RESET;

	//if channel does not exist, create channel
	for (size_t i = 0; i < _channelList.size(); i++)
	{
		std::cout << YELLOW << "channel<" << _channelList[i] << ">\n";
		//
			std::cout << "Available channels: ";
			for(std::map<std::string, Channel>::iterator it = _channels->begin(); it != _channels->end(); ++it)
				std::cout << it->second.getName() << " ";
			std::cout << std::endl;
		//
		// if (!_channels->empty())
		// {
			std::map<std::string, Channel>::iterator it = _channels->find(_channelList[i]);
			// std::cout << "Found channel: " << it->second.getName() << "with key:" << it->second.getKey() << "\n";
			// if doesn't exist, create channel (its ctor adds itself client) 
			if (it == _channels->end())
			{
				Channel newChannel(_channelList[i], client);
				(*_channels)[_channelList[i]] = newChannel;
				// if a key is associated to channel, set Protection mode to channel and add key
				if (i < _keyList.size() && _keyList[i] != "x")
				{
					(*_channels)[_channelList[i]].setChannelProtection(true);
					if (!_keyList[i].empty())
						(*_channels)[_channelList[i]].setKey(_keyList[i]);
				}
			}
			// else add client to existing channel
			else
			{
				std::cout << "trying to login with key\n";
				if (!it->second.getKey().empty())
				{
					// if key is incorrect, cannot join channel and send error
					std::cout << "key:<" << it->second.getKey() << "> trying with:<" << _keyList[i] << ">\n";
                    
					if (_keyList.empty() || (i < _keyList.size() && it->second.getKey() != _keyList[i]))
						return (ERR_BADCHANNELKEY(client.getServerName(), client.getNickname(), it->second.getName()));
				}
				it->second.addConnectedClient(client);
			}
		// }
		// send success (3)
	}

	std::cout << RESET;
	// maybe write method to send message and send it in the loop !! 
	// std::string messageJoin = JOIN_SUCCESS(client.getNickname(), channelName);
	// std::string messageTopic = RPL_TOPIC(client.getNickname(), channelName, )
	// send(client.getClientSocket(), messageJoin.c_str(), messageJoin.length(), 0);
	return "";
}


std::string	Join::parseArgument(Client &client, std::string& arg)
{
	(void)client;
	std::cout << BLUE << "[JOIN - parseArgument]\n" << RESET;
	// irssi client pre-parses arguments: will count only one space to split <#chan, #chan,> <key,key>
	// adds automatically prefix '#' to channels' name.
	// if key is missing, irssi defines key as 'x'
	
	// if arg == "0" ==> Leave all channels (send PART command for each channel) 
	if (arg == "#0")
		return ("PART");
	
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
		if (buffer.size() <= 1)
			return (ERR_BADCHANMASK(buffer));
		_channelList.push_back(buffer);
	}

	// split keyArg with ',' 
	std::stringstream keyStream(keyArg);
	while (std::getline(keyStream, buffer, ','))
		_keyList.push_back(buffer);

	/* DEBUG */
	std::cout << "channelArg:<" << channelArg << ">\n";
	std::cout << "keyArg:<" << keyArg << ">\n";
	std::cout << "channelListSize:<" << _channelList.size() << ">\n";
	std::cout << "keyListSize:<" << _keyList.size() << ">\n";

	for (size_t i = 0; i < _channelList.size(); i++)
		std::cout << YELLOW << _channelList[i] << " ";
	std::cout << RESET << "\n";
	for (size_t i=0; i < _keyList.size(); i++)
		std::cout << GREEN << _keyList[i] << " ";
	std::cout << RESET << "\n";
	
	// pas utile si irssi parse lui meme <#chan1, #chan2> <key1, x> 
	// the list of a channel list must have the same size than the list of the key associated
	// if (_channelList.size() != _keyList.size())
	// 	return "CODE ERROR";
	return "";
}


void	Join::handleRequest(Client &client, std::string arg)
{
	std::cout << BLUE << "[JOIN - handleRequest]\n" << RESET;
	std::string message = "";

	if (arg.empty())
		message = ERR_NEEDMOREPARAMS(client.getServerName(), "JOIN");
	else
	{
		std::string parseResults = parseArgument(client, arg);
		if (!parseResults.empty())
			message = parseResults;
		else
			message = action(client);
	}
	std::cout << "final message:<" << message << ">\n";
	if (message.empty())
	{
		// message = JOIN_SUCCESS(client.getNickname(), _channelList[_channelList.size() -1]);
		// std::string finalmessage = PRIVMSG(client.getServerName(), _channelList[_channelList.size() -1], message);
		// send(client.getClientSocket(), finalmessage.c_str(), finalmessage.length(), 0);
		// _channels.broadcastNumericReplies()
	}
	else
		send(client.getClientSocket(), message.c_str(), message.length(), 0);

	// clear data for next JOIN command 
	std::cout << "before clear: size " << YELLOW << _keyList.size() << RESET << std::endl;

	std::fill(_channelList.begin(), _channelList.end(), "");
	_channelList.clear();
	std::fill(_keyList.begin(), _keyList.end(), "");
	_keyList.clear();
	std::cout << "after clear: size " << YELLOW << _keyList.size() << RESET << std::endl;
}

