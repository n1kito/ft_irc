#include "Invite.hpp"

/* CONSTRUCTORS ***************************************************************/

Invite::Invite() {}
Invite::Invite(ACommand::clientMap* clients) : ACommand(clients) {}
Invite::Invite(ACommand::clientMap* clients, Invite::channelMap* channels) : ACommand(clients), _channelMap(channels) {}

Invite::Invite(const Invite &copyMe) : ACommand() { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Invite::~Invite() { }

/* OVERLOADS ******************************************************************/

Invite& Invite::operator = (const Invite &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void	Invite::parseArgument() {}
void	Invite::parseArgument(std::string arg, std::string& channel, std::string& invitedUser)
{	
	if (arg.find(' ', 0)) // if there is at least a space in the request
	{
		invitedUser = arg.substr(0, arg.find(' ', 0));
		// Once user has been extracted, remove it from the request
		arg.erase(0, invitedUser.length());
		// And move on to extracting channel
		channel = arg.substr(arg.find_first_not_of(' ', 0), std::string::npos);
		// If channel still has spaces in it (because user used double quotes in its request for example),
		// trim channel parameter to first word
		if (channel.find(' ', 0))
			channel = channel.substr(0, channel.find(' ', 0));
	}
	else // there was only one parameter sent, which is username
	// TODO: from my test with irssi, this is only possible if IRSSI knows that user has joined a channel,
	// otherwise not request is sent at all
		invitedUser = arg;
}

void		Invite::handleRequest(Client &client, std::string arg)
{
	std::cout << "Received invite request: <" << arg << ">" << std::endl;
	// Let's be nice and start with removing any extra spaces at the beginning of the request,
	// which our users can add to their requests with things like /invite username "     channel"
	while (!arg.empty() && arg.find_first_of(' ') == 0)
		arg.erase(0, 1);
	// and remove them at the end of the request as well
	while (!arg.empty() && arg.find_last_of(' ') == arg.size() - 1)
		arg.erase(arg.size() - 1, 1);
	// TODO: in my tests, an empty command is not sent to the server, irssi gives the user its own error
	if (arg.empty())
		sendNumericReplies(1, client.getClientSocket(), ERR_NEEDMOREPARAMS(client.getServerName(), "INVITE").c_str());
	else
	{
		std::string	channel = "";
		std::string	invitedUser = "";
		parseArgument(arg, channel, invitedUser);
		std::cout << "Channel is:\t<" << channel << ">" << std::endl;
		std::cout << "User is:\t<" << invitedUser << ">" << std::endl;
		// if invited client is not part of server
		if (getClientByNickname(invitedUser) == NULL)
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_NOSUCHNICK(client.getServerName(), client.getNickname(), invitedUser).c_str());
		// TODO: seems like IRSSI handles this case by itself. If user is not part of any channels an error is thrown
		else if (channel.empty() && client.getCurrentChannel() == NULL)
			sendCustomNumericReply("you are not part of any channels, channel name required", 999, *getClientByNickname(invitedUser));
		// if channel does not exist
		else if (_channelMap->find(channel) == _channelMap->end())
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), channel).c_str());
		else
			action(client, channel, invitedUser, *getClientByNickname(invitedUser));
	}
}

// TODO: find out how to find out if a client is an existing user just from its nickname
// and how to access to its Client instance

void	Invite::action() { }
void	Invite::action(const Client& client, const std::string& channel, const std::string& invitedUserNick, Client& invitedClient)
{
	// If channel name is empty, targetChannel is the user inviting's current channel
	// Otherwise, find channel object in _channelMap
	Channel& targetChannel = channel.empty() ? *client.getCurrentChannel() : (*_channelMap).at(channel);

	// If client inviting is not part of the channel
	if (targetChannel.isClientConnected(client) == false)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), channel).c_str());
	// If channel is invite only and user inviting is not an operator
	else if (targetChannel.isInviteOnly() && targetChannel.isClientOperator(client) == false)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), channel).c_str());
	// If user is already on the channel
	else if (targetChannel.isClientConnected(invitedClient) == true)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_USERONCHANNEL(client.getServerName(), client.getNickname(), invitedUserNick, channel).c_str());
	// Invite is successful, yay !
	else
	{
		if (targetChannel.isInviteOnly())
			targetChannel.addInvitedClient(invitedUserNick);
		sendNumericReplies(1, client.getClientSocket(), \
			RPL_INVITING(client.getServerName(), client.getNickname(), invitedUserNick, channel).c_str());
		sendCustomNumericReply(client.getNickname() + " has invited " + invitedUserNick + " to the channel " + channel, 999, client);
	}
}