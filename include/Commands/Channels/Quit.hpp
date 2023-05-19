/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli <cgosseli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:17:03 by jeepark           #+#    #+#             */
/*   Updated: 2023/05/19 14:19:03 by cgosseli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUIT_HPP
# define QUIT_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "ACommand.hpp"
# include "numericReplies.hpp"
# include "ft_irc.hpp"

class Quit : public ACommand
{
	public:
		typedef std::map< std::string, Channel >	channelMap;
		
		Quit(ACommand::clientMap* clients, channelMap* channels);
		// typedef std::map< std::string, Channel > channelMap;
		// Quit(std::map< int, Client >* clients, std::map< std::string, Channel >* channels);
		~Quit();

		void		handleRequest(Client &client, std::string arg);
		void		parseArgument(Client& client, std::string& arg);
    	void		action(Client &client, std::string& arg);
		
	protected:
		// add protected elements here

	private:
		channelMap*		_channelMap;
		Quit(std::map< int, Client >* clients);
		
	    void			parseArgument();
    	void			action();
		Quit();
		Quit(const Quit &copyMe);
		Quit&		operator = (const Quit &copyMe);
};

#endif
