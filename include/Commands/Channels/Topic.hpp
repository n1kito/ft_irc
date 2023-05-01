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
	
		void				parseArgument(std::string arg, std::string& channel, std::string& topic);
		void				handleRequest(Client &client, std::string arg);
		const Channel&		getChannel() const;
		const Client&		getClient() const;
		void				action();

	protected:

	private:
		Topic();
		void				parseArgument();
		Topic(const Topic &copyMe);
		Topic&				operator = (const Topic &copyMe);

		Channel*			_channel;
		Client*				_client;
};

#endif
