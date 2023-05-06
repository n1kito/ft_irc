#ifndef INVITE_HPP
# define INVITE_HPP

# include "ft_irc.hpp"
# include "ACommand.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "numericReplies.hpp"
# include <string>

class Invite : public ACommand
{
	public:
		typedef std::map< int, Client >				clientMap;
		typedef std::map< std::string, Channel >	channelMap;

		Invite(clientMap* clients);
		Invite(clientMap* clients, channelMap* channels);
		~Invite();
	
		void				parseArgument(std::string arg, std::string& channel, std::string& request);
		void				handleRequest(Client &client, std::string arg);
		void				action();

	protected:

	private:
		Invite();
		void				parseArgument();
		Invite(const Invite &copyMe);
		Invite&				operator = (const Invite &copyMe);

		channelMap*			_channelMap;
};

#endif
