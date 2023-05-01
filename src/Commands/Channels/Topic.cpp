#include "Topic.hpp"

/* CONSTRUCTORS ***************************************************************/

Topic::Topic() {}

Topic::Topic(std::map< int, Client >* clients) : ACommand(clients) {}

Topic::Topic(std::map< int, Client >* clients, Channel* channel, Client* clientRequesting) :
	ACommand(clients),
	_channel(channel),
	_client(clientRequesting)
{}

Topic::Topic(const Topic &copyMe) : ACommand() { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Topic::~Topic() { }

/* OVERLOADS ******************************************************************/

Topic& Topic::operator = (const Topic &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

std::string	Topic::parseArgument(const std::string& arg)
{
	if (arg.empty())
	{
		// If argument is empty but topic is not, send topic
		if (!_channel->getTopic().empty())
			return RPL_TOPIC(_client->getServerName(), _client->getNickname(), _channel->getName(), _channel->getTopic());
		// else, send a message saying there is no topic
		else
			return RPL_NOTOPIC(_client->getServerName(), _client->getNickname(), _channel->getName());
	}
	
	return NULL;
}

void		Topic::handleRequest(Client &client, std::string arg)
{
	std::string	returnMessage = "";

	returnMessage = parseArgument(arg);
	// If there was an error during parsing
	if (!returnMessage.empty())
		send(client.getClientSocket(), returnMessage.c_str(), returnMessage.length(), 0);
	else
	{
		
	}
	
}

const Channel&	Topic::getChannel() const { return *_channel; }
const Client&	Topic::getClient() const { return *_client; }
