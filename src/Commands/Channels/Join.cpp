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

void		Join::action(Client &client)
{
	std::cout << BLUE << "[JOIN - action]\n" << RESET;

	//if channel does not exist, create channel
	for (size_t i=0; i < _channelList.size();i++)
	{
		std::cout << YELLOW << "channel<" << _channelList[i] << ">\n";
		if (!_channels->empty())
		{
			std::map<std::string, Channel>::iterator it = _channels->find(_channelList[i]);
			// if doesn't exist, create channel (its ctor adds itself client) 
			if (it == _channels->end())
				_channels->insert(std::make_pair(_channelList[i], Channel(_channelList[i], client)));
			// else add client to existing channel
			else
				it->second.addConnectedClient(client);
		}

		// send success (3)
	}

	std::cout << RESET;
	// maybe write method to send message and send it in the loop !! 
	// std::string messageJoin = JOIN_SUCCESS(client.getNickname(), channelName);
	// std::string messageTopic = RPL_TOPIC(client.getNickname(), channelName, )
	// send(client.getClientSocket(), messageJoin.c_str(), messageJoin.length(), 0);
}


std::string	Join::parseArgument(Client &client, std::string& arg)
{
	std::cout << BLUE << "[JOIN - parseArgument]\n" << RESET;

	(void)client;
	// if arg == "0" ==> Leave all channels (send PART command for each channel) 
	if (arg == "#0")
		return ("PART");
	std::string channelArg;
	std::string keyArg;

	// split the channel list and the key list with a space
	std::stringstream argStream(arg);
	std::getline(argStream, channelArg, ' ');
	std::getline(argStream, keyArg);
	
	std::string buffer;
	
	std::stringstream channelStream(channelArg);
	while (std::getline(channelStream, buffer, ','))
		_channelList.push_back(buffer);
	
	std::stringstream keyStream(keyArg);
	while (std::getline(keyStream, buffer, ','))
		_keyList.push_back(buffer);

	std::cout << "channelArg:<" << channelArg << ">\n";
	std::cout << "keyArg:<" << keyArg << ">\n";
	std::cout << "channelListSize:<" << _channelList.size() << ">\n";
	std::cout << "keyListSize:<" << _keyList.size() << ">\n";

	for (size_t i=0; i < _channelList.size(); i++)
		std::cout << YELLOW << _channelList[i] << " ";
	std::cout << RESET << "\n";
	for (size_t i=0; i < _keyList.size(); i++)
		std::cout << GREEN << _keyList[i] << " ";
	std::cout << RESET << "\n";
	
	// the list of a channel list must have the same size than the list of the key associated
	if (_channelList.size() != _keyList.size())
		return "CODE ERROR";
	
	return "JOIN_SUCCESS";
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
			action(client);
	}
	std::cout << "final message:<" << message << ">\n";
	// if (!message.empty())
	// message = std::string("PRIVMSG ") + "#c1" + " :" + JOIN_SUCCESS(client.getNickname(), "#c1");
	message = PRIVMSG(_channelList[0], JOIN_SUCCESS(client.getNickname(), _channelList[0]));
	send(client.getClientSocket(), message.c_str(), message.length(), 0);
	
	// clear data 
	_channelList.clear();
	_keyList.clear();
	// on SUCCESS, send all the information needed to the client
}

