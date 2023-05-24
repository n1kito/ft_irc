/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:40:15 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:40:16 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "ft_irc.hpp"
# include "ACommand.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "numericReplies.hpp"
# include <string>

class Topic : public ACommand
{
	public:
		typedef std::map< int, Client >				clientMap;
		typedef std::map< std::string, Channel >	channelMap;

		Topic(clientMap* clients);
		Topic(clientMap* clients, channelMap* channels);
		~Topic();
	
		void				parseArgument(std::string arg, std::string& channel, std::string& topic);
		void				handleRequest(Client &client, std::string arg);
		void				action();
		void				action(std::string& topic, Channel& targetChannel, const Client& client);

	private:
		void				parseArgument();

		channelMap*			_channelMap;

		Topic();
		Topic(const Topic &copyMe);
		Topic&				operator = (const Topic &copyMe);
};

#endif
