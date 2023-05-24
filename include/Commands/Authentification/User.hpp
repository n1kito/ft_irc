/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:42:42 by jeepark           #+#    #+#             */
/*   Updated: 2023/05/24 13:38:57 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "ACommand.hpp"
#include "ft_irc.hpp"
#include "Client.hpp"
#include "numericReplies.hpp"
#include <limits>

class User : public ACommand
{
	public:
		User(const User &copyMe);
		User(std::map< int, Client >* clients );

		~User();
		User&		operator = (const User &copyMe);

		// void
	protected:
		std::string	getUsername() const;
		std::string	getRealname() const;
		void		setUsername( std::string username );
		void		setRealname( std::string realname );
		void		handleRequest( Client& client, std::string argument );
		void		parseArgument();
		std::string	parseArgument( Client& client, std::string argument );
		void		action();
		std::string	action( Client& client, std::string username, std::string realname ); 

	private:
		User();
		std::string	_username;
		std::string	_realname;
		// add private elements here
};

#endif

/*
	Command: USER
  	Parameters: <username> <mode> * <realname>

	modes:
	a - user is flagged as away;
    i - marks a users as invisible;
    w - user receives wallops;
    r - restricted user connection;
    o - operator flag;
    O - local operator flag;
    s - marks a user for receipt of server notices.


	Numeric Replies:
	ERR_NEEDMOREPARAMS (461)
	ERR_ALREADYREGISTERED (462)

	example: 
	USER guest 0 * :Ronnie Reagan 
*/
