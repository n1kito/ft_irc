/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:40:23 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:40:24 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTICE_HPP
# define NOTICE_HPP

# include "ft_irc.hpp"
# include "ACommand.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "numericReplies.hpp"
# include <string>

class Notice : public ACommand
{
	public:
		typedef std::map< std::string, Channel >	channelMap;

		Notice(ACommand::clientMap* clients, channelMap* channels);
		~Notice();
	
		void			handleRequest(Client &client, std::string arg);
		std::string		parseArgument(std::string& arg);
		void			action(Client &client);
		void			sendToChannel(Client& client, Channel& channel, std::string& target);

	protected:
		// add protected elements here

	private:
		std::string					_message;
		std::vector< std::string >	_targetVector;
		channelMap*					_channelMap;
		
		void			parseArgument();
    	void			action();
		Notice();
		Notice(const Notice &copyMe);
		Notice(std::map< int, Client >* clients);
		Notice&			operator = (const Notice &copyMe);
};

#endif
