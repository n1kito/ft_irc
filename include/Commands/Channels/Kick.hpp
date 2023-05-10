/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjallada <mjallada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 17:02:15 by mjallada          #+#    #+#             */
/*   Updated: 2023/05/09 17:02:15 by mjallada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
# define KICK_HPP

# include "ACommand.hpp"
# include "numericReplies.hpp"
# include "ft_irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"


class Kick   : public ACommand
{
	public:
		typedef std::map< std::string, Channel >	channelMap;

		Kick(std::map< int, Client >* clients, std::map< std::string, Channel >* channels);
		~Kick();
	
		void		handleRequest(Client &client, std::string arg);
    	std::string	parseArgument(Client &client, std::string& arg);
    	std::string action(Client &client);

	protected:
		// add protected elements here

	private:
		channelMap*		_channelMap;
		std::string								_kickReason;
		std::vector< std::string >				_channelList;
		std::vector< std::string >				_userList;


		void		parseArgument();
    	void		action();
		Kick();
		Kick(std::map< int, Client >* clients);
		Kick(const Kick &copyMe);
		Kick&		operator = (const Kick &copyMe);
};

#endif
