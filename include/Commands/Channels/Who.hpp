/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:40:17 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:40:18 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHO_HPP
# define WHO_HPP

# include "ACommand.hpp"
# include "ft_irc.hpp"
# include "Channel.hpp"
# include "Client.hpp"

class Client;

class Who : public ACommand
{
	public:
		typedef std::map< std::string, Channel >	channelMap;
		typedef ACommand::clientMap					clientMap;

		Who(clientMap* clients);
		Who(clientMap* clients, channelMap* channels);
		~Who();

    	void		handleRequest(Client &client, std::string arg);
    	void		parseArgument();
    	void		parseArgument(std::string& arg, std::string& target);
    	void		action();
		void		action(const Client& client, const std::string& target);

	private:
		channelMap*	_channelMap;

    	Who();
		Who(const Who &copyMe);
		Who&		operator = (const Who &copyMe);
};

#endif