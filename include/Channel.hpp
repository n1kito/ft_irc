#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include "Client.hpp"
# include <vector>

class Channel
{
	public:
		typedef std::map< std::string, const Client* > clientNickMap;

		Channel( std::string name, const Client& client );
		Channel(const Channel &copyMe);
		~Channel();

		// getters
		std::string				getName() const;
		std::string				getTopic() const;
		const clientNickMap&	getClientMap() const;
		const clientNickMap&	getOperators() const;
		// getters -> channel modes
		bool					isClientLimitMode() const;
		bool					isTopicProtectedMode() const;

		// setters
		void			setName(const std::string& newName);
		void			setTopic(const std::string& newTopic);
		void			setClientLimit(const size_t& limit);
		void			setTopicProtection(const bool& status);
		void			addConnectedClient(const Client& clientRef);
		void			removeConnectedClient(const std::string& clientNickname);
		void			addOperator(Client& clientRef);
		void			removeOperator(const std::string& clientNickname);

		bool			checkTopic(const std::string argument);
		bool			checkName(const std::string name);
	protected:
		// add protected elements here

	private:
		Channel();
		clientNickMap			_connectedClients;
		clientNickMap			_operators;
		std::string				_name;
		std::string				_topic;
		
		// Channel modes
		size_t					_clientLimit;
		bool					_topicIsProtected;

		Channel&		operator = (const Channel &copyMe);

};

#endif
