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
void	Topic::parseArgument(std::string arg, std::string& channel, std::string& topic)
{
	// Let's say our server requires clean commands, a correct command structure is
	// /topic #channel "topic"
	// A topic of multiple words that is not enclosed within quotes will just be cut at the first space.
	//
	// Let's be nice and start with removing any extra spaces at the end of the request
	while (arg.find_last_of(' ') == arg.size() - 1)
	{
		std::cout << "removing space..." << std::endl;
		arg.erase(arg.size() - 1, 1);
	}
	// Extract whatever is before the ':', that's our channel name
	channel = arg.substr(0, arg.find(':'));
	if (!channel.empty() && channel[channel.size() - 1] == ' ')
		 channel.erase(channel.end() - 1);
	// Then extract whatever is after the ':', that's our topic
	topic = arg.substr(arg.find(':') + 1, std::string::npos);
	// If the topic was cleanly enclosed in matching double quotes, we remove them
	if (!topic.empty() && topic[0] == '"' && topic[topic.size() - 1] == '"')
	{
		topic.erase(0, 1);
		topic.erase(topic.size() - 1, 1);
	}
	// Otherwise the topic will be the first token
	else
	{
		if (topic.find(' '))
			topic.erase(topic.find(" "), std::string::npos);
	}
}

// 
void		Topic::handleRequest(Client &client, std::string arg)
{
	//// Here is what a full request looks like:
	// TOPIC #MDR :"new topic"
	//// Missing a topic
	// TOPIC #MDR
	//// With an empty argument
	// TOPIC #MDR ""
	// TODO: there are two cases here, either the topic is not given or the topic is an empty string. In any case I'm not sure how to differentiate the two in this command proto
	// TODO: if the topic was cleared, all clients receive a TOPIC command, see below. Find a way to do this in a clear way.
	_client = &client;
	std::string channel;
	std::string topic;
	parseArgument(arg, channel, topic);
	std::cout	<< "Channel is: <" << channel << ">" << std::endl
			<< "Topic is: <" << topic << ">" << std::endl;
	// If there was an error during parsing, return
	/*
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
	*/
}

void	Topic::action()
{
	/*
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
	*/
}

// Getters
const Channel&	Topic::getChannel() const { return *_channel; }
const Client&	Topic::getClient() const { return *_client; }
