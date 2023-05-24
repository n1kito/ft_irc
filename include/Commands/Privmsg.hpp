/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:40:25 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:40:26 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include "ft_irc.hpp"
# include "ACommand.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "numericReplies.hpp"
# include <string>

class Privmsg : public ACommand
{
	public:
		typedef std::map< std::string, Channel >	channelMap;
		
		Privmsg(ACommand::clientMap* clients, channelMap* channels);
		~Privmsg();
	
		void				handleRequest(Client &client, std::string arg);
		std::string			parseArgument(Client &client, std::string& arg);
		std::string			action(Client &client);
		std::string			sendToChannel(Client& client, Channel& channel, std::string& target);

	protected:
		// add protected elements here

	private:
		std::string					_message;
		std::vector< std::string >	_targetVector;
		channelMap*					_channelMap;
		

		void		parseArgument();
    	void		action();
		Privmsg();
		Privmsg(const Privmsg &copyMe);
		Privmsg(std::map< int, Client >* clients);
		Privmsg&		operator = (const Privmsg &copyMe);

		// add private elements here
};

#endif
