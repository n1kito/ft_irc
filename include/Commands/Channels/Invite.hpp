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
		typedef std::map< std::string, Channel > channelMap;

		Invite(ACommand::clientMap* clients);
		Invite(ACommand::clientMap* clients, channelMap* channels);
		~Invite();
	
		void				parseArgument(std::string arg, std::string& channel, std::string& request);
		void				handleRequest(Client &client, std::string arg);
		void				action(const Client& client, const std::string& channel, const std::string& invitedUserNick, Client& invitedClient);

	private:
		void				parseArgument();
		void				action();
		
		channelMap*			_channelMap;

		Invite();
		Invite(const Invite &copyMe);
		Invite&				operator = (const Invite &copyMe);
};

#endif
