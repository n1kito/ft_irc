/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli <cgosseli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 14:47:16 by cgosseli          #+#    #+#             */
/*   Updated: 2023/04/25 15:09:28 by cgosseli         ###   ########.fr       */
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
		Ping( const std::map< int, Client >* clients );
		
		~Ping();
		Ping&		operator = (const Ping &copyMe);
	
	protected:

		std::string	handleRequest( Client& client, std::string argument );
		void		parseArgument();
		std::string	parseArgument( Client& client, std::string argument );
		void		action();
		// add protected elements here

	private:
		Ping();
		// add private elements here
};

#endif
