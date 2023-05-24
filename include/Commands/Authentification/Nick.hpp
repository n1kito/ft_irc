/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 17:39:23 by jeepark           #+#    #+#             */
/*   Updated: 2023/05/24 13:38:54 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef NICK_HPP
# define NICK_HPP

#include "ACommand.hpp"
#include "ft_irc.hpp"
#include "Client.hpp"
#include <map>

class Client;

class Nick : public ACommand
{
	public:
		Nick(std::map< int, Client >* clients);
		~Nick();

    	void		handleRequest(Client &client, std::string arg);
    	std::string	parseArgument(Client &client, std::string& arg);
    	std::string	action(Client &client, std::string nickname);
		bool		isValidNickname(std::string nickname);

	protected:

	private:
    	Nick();
		Nick(const Nick &copyMe);
		Nick&		operator = (const Nick &copyMe);
		void		parseArgument();
    	void		action();

};

#endif
