#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "ft_irc.hpp"
# include "ACommand.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "numericReplies.hpp"
# include <string>

class Topic : public ACommand
{
	public:
		Topic(std::map<int, Client>* clients);
		~Topic();
	
		bool				parseArgument(const std::string& arg);
		void				handleRequest(Client &client, std::string arg, Channel* channel);
		const Channel&		getChannel() const;
		const Client&		getClient() const;

	protected:

	private:
		Topic();
		void				parseArgument();
		void				handleRequest(Client &client, std::string arg);
		Topic(const Topic &copyMe);
		Topic&				operator = (const Topic &copyMe);

		Channel*			_channel;
		Client*				_client;
};

#endif
