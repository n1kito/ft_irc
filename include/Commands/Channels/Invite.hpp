#ifndef INVITE_HPP
# define INVITE_HPP

# include "ft_irc.hpp"
# include "ACommand.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "numericReplies.hpp"
# include <string>

/*
The INVITE command is used to invite a user to a channel.
Parameters: <nickname> <channel>
Requests:
	/invite nickname channel			=>	INVITE nickname channel
	/invite nickname channel channel	=>	INVITE nickname channel
	/invite								=>	Error from irssi: Not enough parameters given
	/invite nickname					=>	Error from irssi: Not joined to any channel
	/invite nickname (when joined)		=>	? What happens here ? TBD
*/

class Invite : public ACommand
{
	public:
		typedef std::map< std::string, Channel >	channelMap;

		Invite(ACommand::clientMap* clients);
		Invite(ACommand::clientMap* clients, channelMap* channels);
		~Invite();
	
		void				parseArgument(std::string arg, std::string& channel, std::string& request);
		void				handleRequest(Client &client, std::string arg);
		void				action(const Client& client, const std::string& channel, const std::string& invitedUserNick, Client& invitedClient);
	
	protected:

	private:
		Invite();
		void				parseArgument();
		void				action();
		Invite(const Invite &copyMe);
		Invite&				operator = (const Invite &copyMe);

		channelMap*			_channelMap;
};

#endif
