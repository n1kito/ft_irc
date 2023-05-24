/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:42:42 by mjallada          #+#    #+#             */
/*   Updated: 2023/05/24 13:39:09 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invite.hpp"

/* CONSTRUCTORS ***************************************************************/

Invite::Invite() {}
Invite::Invite(ACommand::clientMap* clients) : ACommand(clients) {}
Invite::Invite(ACommand::clientMap* clients, Invite::channelMap* channels) : ACommand(clients), _channelMap(channels) {}
Invite::Invite(const Invite &copyMe) : ACommand() { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Invite::~Invite() {}

/* OVERLOADS ******************************************************************/

Invite& 	Invite::operator = (const Invite &copyMe)
{
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Invite::parseArgument() {}
void		Invite::parseArgument(std::string arg, std::string& channel, std::string& invitedUser)
{	
	std::stringstream	argStream(arg);

	argStream >> invitedUser;
	argStream >> channel;
}

void		Invite::handleRequest(Client &client, std::string arg)
{
	std::string	channel = "";
	std::string	invitedUser = "";
	parseArgument(arg, channel, invitedUser);
	if (channel.empty() || invitedUser.empty())
		sendNumericReplies(1, client.getClientSocket(), ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "INVITE").c_str());
	else if (getClientByNickname(invitedUser) == NULL) // user is not part of the channel
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_NOSUCHNICK(client.getServerName(), client.getNickname(), invitedUser).c_str());
	else if (_channelMap->find(channel) == _channelMap->end())
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), channel).c_str());
	else
		action(client, channel, invitedUser, *getClientByNickname(invitedUser));
}

void		Invite::action() { }
void		Invite::action(const Client& client, const std::string& channel, const std::string& invitedUserNick, Client& invitedClient)
{
	Channel& targetChannel = (*_channelMap).at(channel);

	if (targetChannel.isClientConnected(client) == false)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_NOTONCHANNEL(client.getServerName(), client.getNickname(), channel).c_str());
	else if (targetChannel.modeIs("invite-only") && targetChannel.isClientOperator(client) == false)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), channel).c_str());
	else if (targetChannel.isClientConnected(invitedClient) == true)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_USERONCHANNEL(client.getServerName(), client.getNickname(), invitedUserNick, channel).c_str());
	else
	{
		if (targetChannel.modeIs("invite-only"))
			targetChannel.addInvitedClient(invitedUserNick);
		sendNumericReplies(1, client.getClientSocket(), \
			RPL_INVITING(client.getServerName(), client.getNickname(), invitedUserNick, channel).c_str());
		sendNumericReplies(1, invitedClient.getClientSocket(), \
			INVITE(client.getNickname(), invitedUserNick, channel).c_str());
	}
}