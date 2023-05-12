/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli <cgosseli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:42:11 by cgosseli          #+#    #+#             */
/*   Updated: 2023/05/15 16:17:21 by cgosseli         ###   ########.fr       */
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
