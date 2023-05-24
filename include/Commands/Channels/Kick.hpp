#ifndef KICK_HPP
# define KICK_HPP

# include "ACommand.hpp"
# include "numericReplies.hpp"
# include "ft_irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class Kick   : public ACommand
{
	public:
		typedef std::map< std::string, Channel >	channelMap;

		Kick(std::map< int, Client >* clients, std::map< std::string, Channel >* channels);
		~Kick();
	
		void									handleRequest(Client &client, std::string arg);
    	std::string								parseArgument(Client &client, std::string& arg);
    	std::string 							action(Client &client);

	private:
		void									parseArgument();
    	void									action();

		channelMap*								_channelMap;
		std::string								_kickReason;
		std::vector< std::string >				_channelList;
		std::vector< std::string >				_userList;

		Kick();
		Kick(std::map< int, Client >* clients);
		Kick(const Kick &copyMe);
		Kick&									operator = (const Kick &copyMe);
};

#endif
