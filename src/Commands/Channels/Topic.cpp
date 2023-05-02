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
	if (arg.empty())
		sendNumericReplies(1, client.getClientSocket(), ERR_NEEDMOREPARAMS(client.getServerName(), "TOPIC").c_str());
	else
	{
		std::string channel = "";
		std::string topic = "";
		parseArgument(arg, channel, topic);
		if (_channelMap->find(channel) == _channelMap->end())
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), channel).c_str());
		else
			action(topic, _channelMap->at(channel), client);
	}
}

void	Topic::action() { }
void	Topic::action(std::string& topic, Channel& targetChannel, const Client& client)
{
	// If the request has no topic, this mean the client wants to know the channel topic
	if (topic.empty())
	{
		// If the client is not connected to the channel
		if (targetChannel.isClientConnected(client) == false)
			sendNumericReplies(1, client.getClientSocket(),
					ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), targetChannel.getName()).c_str());
		else 
		{
			// If the channel topic is not empty, send current topic (2 messages as per documentation)
			if (!targetChannel.getTopic().empty())
				sendNumericReplies(2, client.getClientSocket(),
					RPL_TOPIC(client.getServerName(), client.getNickname(), targetChannel.getName(), targetChannel.getTopic()).c_str(),
					RPL_TOPICWHOTIME(client.getServerName(), client.getNickname(), targetChannel.getName(), targetChannel.getNicknameOfTopicSetter(), targetChannel.getTimeTopicWasSet()).c_str());
			// else, send a message saying there is no topic
			else
				sendNumericReplies(1, client.getClientSocket(), \
				RPL_NOTOPIC(client.getServerName(), client.getNickname(), targetChannel.getName()).c_str());
		}
	}
	else
	{
		// If user tried to clear the topic, empty the topic variable
		if (topic == "\"\"")
			topic = "";
		// Can only update if channel topic is not protected or user is an Operator
		if (targetChannel.isTopicProtectedMode() == false || targetChannel.isClientOperator(client))
		{
			// Update channel properties
			targetChannel.setTimeTopicWasSet(getCurrentDate());
			targetChannel.setNicknameOfTopicSetter(client.getNickname());
			targetChannel.setTopic(topic);
			// Broadcast new topic to all users in the channel, followd by a RPL_TOPICWHOTIME
			targetChannel.broadcastNumericReply(RPL_TOPIC(client.getServerName(), client.getNickname(), targetChannel.getName(), targetChannel.getTopic()));
			targetChannel.broadcastNumericReply(RPL_TOPICWHOTIME(client.getServerName(), client.getNickname(), targetChannel.getName(), targetChannel.getNicknameOfTopicSetter(), targetChannel.getTimeTopicWasSet()));
		}
		else
		{
			// Tell user he cannot update the channel topic. Sorry user.
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), targetChannel.getName()).c_str());
		}
	}
}