/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:40:19 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:40:20 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include <string>
# include "Client.hpp"
# include <map>
# include <sys/socket.h>
# include <unistd.h>
# include "numericReplies.hpp"

class Client;

class ACommand
{
	public:
		typedef std::map< int, Client >	clientMap;
		ACommand();
		ACommand(clientMap* clients);
		ACommand(const ACommand &copyMe);
		virtual ~ACommand();
		ACommand&			operator = (const ACommand &copyMe);
	
		virtual void		handleRequest(Client &client, std::string arg) = 0;

		// getters
		Client*				getClientByNickname(const std::string& nickname);

	protected:
		virtual void		parseArgument() = 0;
		virtual void		action() = 0;
		void				killClient(int fd, std::string previousMsg, std::string errorMsg);
		// void				cleanRequestSpaces(std::string& request);

		clientMap* 			_clients;

	private:
		// add private elements here
};

#endif