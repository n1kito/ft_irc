/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:42:42 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:38:55 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_HPP
# define PING_HPP

#include "ACommand.hpp"
#include "ft_irc.hpp"
#include "numericReplies.hpp"

class Ping : public ACommand
{
	public:
		Ping(const Ping &copyMe);
		Ping( std::map< int, Client >* clients );
		
		~Ping();
		Ping&		operator = (const Ping &copyMe);
	
	protected:

		void		handleRequest( Client& client, std::string argument );
		void		parseArgument();
		std::string	parseArgument( Client& client, std::string argument );
		void		action();
		// add protected elements here

	private:
		Ping();
		// add private elements here
};

#endif
