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
