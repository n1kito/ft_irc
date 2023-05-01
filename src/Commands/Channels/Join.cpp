#include "Join.hpp"

/* CONSTRUCTORS ***************************************************************/

Join::Join() {}

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

void		Join::createChannel(Client& client, std::string channelName)
{
	Channel newChannel(channelName, client);
	_channels->insert(std::make_pair(channelName, newChannel));
}

void		Join::action(Client &client)
{
	//if channel does not exist, create channel
	for (size_t i=0; i < _channelsToJoin.size();i++)
	{
		std::map<std::string, Channel>::iterator it = _channels->find(_channelsToJoin[i]);
		// add client to existing channel
		if (it != _channels->end())
			it->second.addConnectedClient(client);
			// add client to existing channel
		else
			createChannel(client, _channelsToJoin[i]);
			// send success (3)
	}
	// maybe write method to send message and send it in the loop !! 
	// std::string messageJoin = JOIN_SUCCESS(client.getNickname(), channelName);
	// std::string messageTopic = RPL_TOPIC(client.getNickname(), channelName, )
	// send(client.getClientSocket(), messageJoin.c_str(), messageJoin.length(), 0);
}


std::string	Join::parseArgument(Client &client, std::string& arg)
{
	(void)client;
	// if arg == "0" ==> Leave all channels (send PART command for each channel) 
	std::map< std::string, std::string > chanKeyMap;
	std::vector < std::string > channelList;
	std::vector < std::string > keyList;
	std::string channelArg;
	std::string keyArg;

	// split the channel list and the key list with a space
	channelArg = arg.substr(0, arg.find(" "));
	keyArg = arg.substr(arg.find(" "));

	size_t pos = channelArg.find(",");
	while ( pos != std::string::npos )
	{
		channelList.push_back(channelArg.substr(0, pos));
		channelArg.erase(0, pos + 1);
		pos = channelArg.find(",");
	}
	pos = keyArg.find(",");
	while ( pos != std::string::npos )
	{
		keyList.push_back(keyArg.substr(0, pos));
		keyArg.erase(0, pos + 1);
		pos = keyArg.find(",");
	}
	// the list of a channel list must have the same size than the list of the key associated
	if (channelList.size() != keyList.size())
		return "CODE ERROR";
	return NULL;
}


void	Join::handleRequest(Client &client, std::string arg)
{
	std::string message = "";

	if (arg.empty())
		message = ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "JOIN");
	else
	{
		std::string parseResults = parseArgument(client, arg);
		if (!parseResults.empty())
			message = parseResults;
		else
			action(client);
	}
	if (!message.empty())
		send(client.getClientSocket(), message.c_str(), message.length(), 0);
	// on SUCCESS, send all the information needed to the client
}

