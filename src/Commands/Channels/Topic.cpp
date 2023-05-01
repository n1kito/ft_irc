#include "Topic.hpp"

/* CONSTRUCTORS ***************************************************************/

Topic::Topic() {}

Topic::Topic(std::map<int, Client>* clients) : ACommand(clients) {}

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

void	Topic::parseArgument() {}
bool	Topic::parseArgument(const std::string& arg)
{
	if (arg.empty())
	{
		// If argument is empty but topic is not, send topic
		if (!_channel->getTopic().empty())
		{
			if (_channel->isClientConnected(*_client) == false)
				sendNumericReplies(1, _client->getClientSocket(),
					ERR_NOTONCHANNEL(_client->getServerName(), _client->getNickname(), _channel->getName()));
			else
			{
				sendNumericReplies(2, _client->getClientSocket(),
					RPL_TOPIC(_client->getServerName(), _client->getNickname(), _channel->getName(), _channel->getTopic()),
					RPL_TOPICWHOTIME(_client->getServerName(), _client->getNickname(), _channel->getName(), _channel->getNicknameOfTopicSetter(), _channel->getTimeTopicWasSet()));
			}
		}
		// else, send a message saying there is no topic
		else
			sendNumericReplies(1, _client->getClientSocket(), \
				RPL_NOTOPIC(_client->getServerName(), _client->getNickname(), _channel->getName()));
		return false;
	}
	return true;
}

// Set to private
void		Topic::handleRequest(Client &client, std::string arg)
{
	(void)client;
	(void)arg;
}

// 
void		Topic::handleRequest(Client &client, std::string arg, Channel* channel)
{
	_channel = channel;
	_client = &client;
	// TODO: there are two cases here, either the topic is not given or the topic is an empty string. In any case I'm not sure how to differentiate the two in this command proto
	// TODO: if the topic was cleared, all clients receive a TOPIC command, see below. Find a way to do this in a clear way.
	std::string	returnMessage = "";
	if (!parseArgument(arg))
		return;
	// If there was an error during parsing, return
	else
	{
		if (_channel->isTopicProtectedMode() && _channel->isClientOperator(*_client) == false)
		{
			sendNumericReplies(1, _client->getClientSocket(), \
				ERR_CHANOPRIVSNEEDED(_client->getServerName(), _client->getNickname(), _channel->getName()));
			return;
		}
		_channel->setTimeTopicWasSet(getCurrentDate());
		_channel->setNicknameOfTopicSetter(_client->getNickname());
		_channel->setTopic(arg);
		// TODO: send new topic via TOPIC command to every client in the channel (including the author of the change)
	}	
}

const Channel&	Topic::getChannel() const { return *_channel; }
const Client&	Topic::getClient() const { return *_client; }
