#include "Topic.hpp"

/* CONSTRUCTORS ***************************************************************/

Topic::Topic() {}
Topic::Topic(Topic::clientMap* clients) : ACommand(clients) {}
Topic::Topic(Topic::clientMap* clients, Topic::channelMap* channels) : ACommand(clients), _channelMap(channels) {}

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
		arg.erase(arg.size() - 1, 1);
	// Check that there has been a topic value entered, if not store the channel name
	if (arg.find(':') == std::string::npos)
		channel = arg;
	else
	{
		// Extract whatever is before the ':', that's our channel name
		channel = arg.substr(0, arg.find(':'));
		if (!channel.empty() && channel[channel.size() - 1] == ' ')
			channel.erase(channel.end() - 1);
		// Then extract whatever is after the ':', that's our topic
		topic = arg.substr(arg.find(':') + 1, std::string::npos);
		// If the topic was just "", that means the user wants to clear the topic so we keep it as is
		if (topic == "\"\"")
			return ;
		// If the topic was cleanly enclosed in matching double quotes, we remove them
		else if (!topic.empty() && topic[0] == '"' && topic[topic.size() - 1] == '"')
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
}

// 
void		Topic::handleRequest(Client &client, std::string arg)
{
	_client = &client;
	if (arg.empty())
		sendNumericReplies(1, _client->getClientSocket(), ERR_NEEDMOREPARAMS(_client->getServerName(), _client->getNickname(), "TOPIC").c_str());
	else
	{
		std::string channel = "";
		std::string topic = "";
		parseArgument(arg, channel, topic);
		std::cout	<< "Channel is: <" << channel << ">" << std::endl
				<< "Topic is: <" << topic << ">" << std::endl;
		if (_channelMap->find(channel) == _channelMap->end())
			sendNumericReplies(1, _client->getClientSocket(), \
				ERR_NOSUCHCHANNEL(_client->getServerName(), _client->getNickname(), channel).c_str());
		else
			action(topic, _channelMap->at(channel));
	}
}

void	Topic::action() { }
void	Topic::action(std::string& topic, const Channel& targetChannel)
{
	// If the request has no topic
	if (topic.empty())
	{
		// If the target channel has one
		if (!targetChannel.getTopic().empty())
		{
			// Is the client is not connected to said channel, send an error
			if (targetChannel.isClientConnected(*_client) == false)
				sendNumericReplies(1, _client->getClientSocket(),
					ERR_NOTONCHANNEL(_client->getServerName(), _client->getNickname(), targetChannel.getName()).c_str());
			// Else, send current topic (2 messages)
			else
				sendNumericReplies(2, _client->getClientSocket(),
					RPL_TOPIC(_client->getServerName(), _client->getNickname(), targetChannel.getName(), targetChannel.getTopic()).c_str(),
					RPL_TOPICWHOTIME(_client->getServerName(), _client->getNickname(), targetChannel.getName(), targetChannel.getNicknameOfTopicSetter(), targetChannel.getTimeTopicWasSet()).c_str());
		}
		// else, send a message saying there is no topic
		sendNumericReplies(1, _client->getClientSocket(), \
			RPL_NOTOPIC(_client->getServerName(), _client->getNickname(), targetChannel.getName()).c_str());
	}
	else
	{
		// If user tried to clear the topic, empty the topic variable
		if (topic == "\"\"")
			topic = "";
		// Update channel properties
		_channel->setTimeTopicWasSet(getCurrentDate());
		_channel->setNicknameOfTopicSetter(_client->getNickname());
		_channel->setTopic(topic);
		// Broadcast new topic to all users in the channel
		_channel->broadcastNumericReply(RPL_TOPIC(_client->getServerName(), _client->getNickname(), targetChannel.getName(), targetChannel.getTopic()));
	}
}

// Getters
const Channel&	Topic::getChannel() const { return *_channel; }
const Client&	Topic::getClient() const { return *_client; }
