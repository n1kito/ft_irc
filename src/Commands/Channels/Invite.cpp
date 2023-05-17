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
	std::stringstream	argStream(arg);

	argStream >> invitedUser;
	argStream >> channel;
}

void		Invite::handleRequest(Client &client, std::string arg)
{
	std::cout << "Received invite request: <" << arg << ">" << std::endl;
	std::string	channel = "";
	std::string	invitedUser = "";
	parseArgument(arg, channel, invitedUser);
	std::cout << "User is:\t<" << invitedUser << ">" << std::endl;
	std::cout << "Channel is:\t<" << channel << ">" << std::endl;
	// if no channel was provided
	if (channel.empty() || invitedUser.empty())
		sendNumericReplies(1, client.getClientSocket(), ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "INVITE").c_str());
	// if invited client is not part of server
	else if (getClientByNickname(invitedUser) == NULL)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_NOSUCHNICK(client.getServerName(), client.getNickname(), invitedUser).c_str());
	// if channel does not exist
	else if (_channelMap->find(channel) == _channelMap->end())
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), channel).c_str());
	else
		action(client, channel, invitedUser, *getClientByNickname(invitedUser));
}

// TODO: find out how to find out if a client is an existing user just from its nickname
// and how to access to its Client instance

void	Invite::action() { }
void	Invite::action(const Client& client, const std::string& channel, const std::string& invitedUserNick, Client& invitedClient)
{
	// If channel name is empty, targetChannel is the user inviting's current channel
	// Otherwise, find channel object in _channelMap
	Channel& targetChannel = (*_channelMap).at(channel);

	// If client inviting is not part of the channel
	if (targetChannel.isClientConnected(client) == false)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), channel).c_str());
	// If channel is invite only and user inviting is not an operator
	else if (targetChannel.modeIs("invite-only") && targetChannel.isClientOperator(client) == false)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), channel).c_str());
	// If user is already on the channel
	else if (targetChannel.isClientConnected(invitedClient) == true)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_USERONCHANNEL(client.getServerName(), client.getNickname(), invitedUserNick, channel).c_str());
	// Invite is successful, yay !
	else
	{
		if (targetChannel.modeIs("invite-only"))
			targetChannel.addInvitedClient(invitedUserNick);
		sendNumericReplies(1, client.getClientSocket(), \
			RPL_INVITING(client.getServerName(), client.getNickname(), invitedUserNick, channel).c_str());
		// sendCustomNumericReply(client.getNickname() + " has invited " + invitedUserNick + " to the channel " + channel, 999, client);
		sendNumericReplies(1, invitedClient.getClientSocket(), \
			INVITE(client.getNickname(), invitedUserNick, channel).c_str());
	}
}