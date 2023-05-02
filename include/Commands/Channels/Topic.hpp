#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "ft_irc.hpp"
# include "ACommand.hpp"
# include "Client.hpp"
#
# include "Channel.hpp"
# include "numericReplies.hpp"
# include <string>

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
		const Channel&		getChannel() const;
		const Client&		getClient() const;
		void				action();
		void				action(std::string& topic, const Channel& targetChannel);

	protected:

	private:
		Topic();
		void				parseArgument();
		Topic(const Topic &copyMe);
		Topic&				operator = (const Topic &copyMe);

		channelMap*			_channelMap;
		// TODO: pas sur d'en avoir besoin de ceux la
		Channel*			_channel;
		Client*				_client;
};

#endif
