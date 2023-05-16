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

// Checks format the request
void	Topic::parseArgument() {}
void	Topic::parseArgument(std::string arg, std::string& channel, std::string& topic)
{	
	// Check that there has been a topic value entered, if not store the channel name
	if (arg.find(':') == std::string::npos)
		channel = arg;
	else
	{
		// Extract whatever is before the ':', that's our channel name
		channel = arg.substr(0, arg.find(':') - 1);
		// Then extract whatever is after the ':', that's our topic
		topic = arg.substr(arg.find(':') + 1, std::string::npos);
		// If the topic was just "", that means the user wants to clear the topic so we keep it as is
		if (topic == "\"\"")
			return ;
	}
}

// Parses the request and handles it accordingly
void		Topic::handleRequest(Client &client, std::string arg)
{
	if (arg.empty())
		sendNumericReplies(1, client.getClientSocket(), ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "TOPIC").c_str());
	else
	{
		std::string channel = "";
		std::string topic = "";
		parseArgument(arg, channel, topic);
		std::cout << "channel is: <" << channel << ">" << std::endl;
		std::cout << "topic is: <" << topic << ">" << std::endl;
		if (_channelMap->find(channel) == _channelMap->end())
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), channel).c_str());
		else
			action(topic, _channelMap->at(channel), client);
	}
}

// Handles the request and sends necessary numeric replies
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
		if (targetChannel.modeIs("protected-topic") == false || targetChannel.isClientOperator(client))
		{
			// Update channel properties
			targetChannel.setTimeTopicWasSet(getCurrentDate());
			// std::cout << "Time topic was set: " << targetChannel.getTimeTopicWasSet() << std::endl;
			targetChannel.setNicknameOfTopicSetter(client.getNickname());
			targetChannel.setTopic(topic);
			// Broadcast new topic to all users in the channel, followd by a RPL_TOPICWHOTIME
			targetChannel.broadcastNumericReply(RPL_TOPIC(client.getServerName(), client.getNickname(), targetChannel.getName(), targetChannel.getTopic()));
		}
		else
		{
			// Tell user he cannot update the channel topic. Sorry user.
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), targetChannel.getName()).c_str());
		}
	}
}