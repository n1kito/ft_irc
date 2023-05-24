/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:42:42 by jeepark           #+#    #+#             */
/*   Updated: 2023/05/24 13:39:02 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PART_HPP
# define PART_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "ACommand.hpp"
# include "numericReplies.hpp"
# include "ft_irc.hpp"

class Part : public ACommand
{
	public:
		Part(std::map< int, Client >* clients, std::map< std::string, Channel >* channels);
		~Part();

    	void		handleRequest(Client &client, std::string arg);
    	void		parseArgument(std::string& arg);
    	void		action(Client &client);

	
	protected:
		// add protected elements here

	private:
		std::map< std::string, Channel >*		_channels;
		std::vector< std::string >				_channelList;
		std::string								_reason;

    	void		parseArgument();
    	void		action();
		Part();
		Part(std::map< int, Client >* clients);
		Part(const Part &copyMe);
		Part&		operator = (const Part &copyMe);

};

#endif
