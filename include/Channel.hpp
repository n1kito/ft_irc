#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <strings>
# include "Client.hpp"

class Channel
{
	public:
		Channel();
		~Channel();

		// setters
		std::string	getName() const;
		std::string	getTopic() const;

		// getters
		void		setName(const std::string& newName);
		void		setTopic(const std::string& newTopic);

	protected:
		// add protected elements here

	private:
		std::map< std::string, Client* > _connectedClients;
		std::string	_name;
		// _mode;
		std::string	_topic;
		// _operators;


		Channel(const Channel &copyMe);
		Channel&		operator = (const Channel &copyMe);

};

#endif
