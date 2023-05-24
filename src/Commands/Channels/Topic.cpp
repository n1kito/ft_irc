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
	if (arg.find(':') == std::string::npos)
		channel = arg;
	else
	{
		channel = arg.substr(0, arg.find(':') - 1);
		topic = arg.substr(arg.find(':') + 1, std::string::npos);
		// If the topic was just "", that means the user wants to clear the topic so we keep it as is
		if (topic == "\"\"")
			return;
	}
}

void		Topic::handleRequest(Client &client, std::string arg)
{
	if (arg.empty())
		sendNumericReplies(1, client.getClientSocket(), ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "TOPIC").c_str());
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
	if (topic.empty())
	{
		if (targetChannel.isClientConnected(client) == false)
			sendNumericReplies(1, client.getClientSocket(),
					ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), targetChannel.getName()).c_str());
		else 
		{
			if (!targetChannel.getTopic().empty())
				sendNumericReplies(2, client.getClientSocket(),
					RPL_TOPIC(client.getServerName(), client.getNickname(), targetChannel.getName(), targetChannel.getTopic()).c_str(),
					RPL_TOPICWHOTIME(client.getServerName(), client.getNickname(), targetChannel.getName(), targetChannel.getNicknameOfTopicSetter(), targetChannel.getTimeTopicWasSet()).c_str());
			else
				sendNumericReplies(1, client.getClientSocket(), \
				RPL_NOTOPIC(client.getServerName(), client.getNickname(), targetChannel.getName()).c_str());
		}
	}
	else
	{
		if (topic == "\"\"")
			topic = "";
		else if (topic.length() > TOPICLEN)
			topic = topic.substr(0, TOPICLEN);
		if (targetChannel.modeIs("topic-protected") == false || targetChannel.isClientOperator(client))
		{
			targetChannel.setTimeTopicWasSet(getCurrentDate());
			targetChannel.setNicknameOfTopicSetter(client.getNickname());
			targetChannel.setTopic(topic);
			targetChannel.broadcastNumericReply(TOPIC_SUCCESS(client.getServerName(), client.getNickname(), targetChannel.getName(), targetChannel.getTopic()));
		}
		else
		{
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), targetChannel.getName()).c_str());
		}
	}
}