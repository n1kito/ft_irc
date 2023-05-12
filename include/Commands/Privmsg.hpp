/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:42:11 by cgosseli          #+#    #+#             */
/*   Updated: 2023/05/12 17:42:11 by cgosseli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

class Privmsg
{
	public:
		Privmsg();
		Privmsg(const Privmsg &copyMe);
		~Privmsg();
		Privmsg&		operator = (const Privmsg &copyMe);
	
	protected:
		// add protected elements here

	private:
		// add private elements here
};

#endif
