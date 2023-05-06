#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "ft_irc.hpp"
# include "ACommand.hpp"
# include "Client.hpp"
#
# include "Channel.hpp"
# include "numericReplies.hpp"
# include <string>

// The TOPIC command is used to change or view the topic of the given channel.

class Topic : public ACommand
{
	public:
		typedef std::map< int, Client >				clientMap;
		typedef std::map< std::string, Channel >	channelMap;

		Topic(clientMap* clients);
		Topic(clientMap* clients, channelMap* channels);
		~Topic();
	
		void				parseArgument(std::string arg, std::string& channel, std::string& topic);
		void				handleRequest(Client &client, std::string arg);
		void				action();
		void				action(std::string& topic, Channel& targetChannel, const Client& client);

	protected:

	private:
		Topic();
		void				parseArgument();
		Topic(const Topic &copyMe);
		Topic&				operator = (const Topic &copyMe);

		channelMap*			_channelMap;
};

#endif
