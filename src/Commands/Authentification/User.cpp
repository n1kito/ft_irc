/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:42:42 by jeepark           #+#    #+#             */
/*   Updated: 2023/05/24 13:39:07 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/* CONSTRUCTORS ***************************************************************/

User::User() : ACommand() {}

User::User(std::map< int, Client >* clients) : ACommand(clients) {}

User::User(const User &copyMe) : ACommand() { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

User::~User() {}

/* OVERLOADS ******************************************************************/

User& User::operator = (const User &copyMe)
{
	(void)copyMe;
	return *this;
}

/* ACCESSORS ******************************************************************/

std::string	User::getUsername() const { return _username; }
std::string	User::getRealname() const { return _realname; }
void		User::setUsername(std::string username) { _username = username; }
void		User::setRealname(std::string realname) { _realname = realname; }

/* METHODS ********************************************************************/

void		User::handleRequest(Client& client, std::string argument)
{
	if (client.getRegisterState())
	{
		sendNumericReplies(1, client.getClientSocket(),
			ERR_ALREADYREGISTERED(client.getServerName(), client.getNickname()).c_str());
		return ;
	}
	std::string message = "";
	std::string ret_parsing = parseArgument(client, argument);
	std::string ret_action = action(client, _username, _realname);
	if (!ret_parsing.empty())
		message = ret_parsing;
	else if (!ret_action.empty())
		message = ret_action;
	else
	{
		if (client.getNickname().empty() == false && client.getPassword().empty() == false)
			client.setRegisterState(true);
		message = USER_SUCCESS(client.getServerName(), client.getNickname());
	}
	if (!message.empty())
		sendNumericReplies(1, client.getClientSocket(), message.c_str());
}

void		User::parseArgument() {}

std::string	User::parseArgument(Client& client, std::string argument)
{
	std::stringstream	iss(argument);
	
	iss >> _username;
	iss.ignore(std::numeric_limits< std::streamsize >::max(), ' ');
	if (_username.size() > USERLEN)
		_username = _username.substr(0, USERLEN);
	// hostname and servername are typically ignored when USER comes from a client
	iss.ignore(std::numeric_limits< std::streamsize >::max(), ' ');
	iss.ignore(std::numeric_limits< std::streamsize >::max(), ' ');

	char tmp_name[REALNAMELEN];
	iss.getline(tmp_name, REALNAMELEN);
	_realname = tmp_name;
	if (_realname[0] == ':')
		_realname.erase(0, 1);
	if (_username.empty() || _realname.empty())
		return (ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "USER"));
	return "";
}

void		User::action() {}
std::string	User::action(Client& client, std::string username, std::string realname)
{
	if (client.getRegisterState())
		return (ERR_ALREADYREGISTERED(client.getServerName(), client.getNickname()));
	client.setUsername(username);
	client.setRealname(realname);
	return "";
}
