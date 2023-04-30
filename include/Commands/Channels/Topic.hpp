#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "ACommand.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "numericReplies.hpp"
# include <string>

class Topic : public ACommand
{
	public:
		Topic(std::map< int, Client >* clients, Channel* channel, Client* clientRequesting);
		~Topic();
	
		std::string			parseArgument(const std::string& arg);
		void				handleRequest(Client &client, std::string arg);
		const Channel&		getChannel() const;
		const Client&		getClient() const;

	protected:

	private:
		Topic();
		Topic(std::map<int, Client>* clients);
		Topic(const Topic &copyMe);
		Topic&		operator = (const Topic &copyMe);

		Channel*	_channel;
		Client*		_client;
};

#endif
