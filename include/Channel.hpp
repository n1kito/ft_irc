#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include "Client.hpp"
# include <vector>
# include <cstdarg>
# include <sys/types.h>
# include <sys/socket.h>
# include "numericReplies.hpp"
# include "ft_irc.hpp"

class Channel
{
	public:
		typedef std::map< std::string, const Client* > clientNickMap;

		Channel( std::string name, const Client& client );
		Channel(const Channel &copyMe);
		Channel&		operator = (const Channel &copyMe);
		Channel();
		~Channel();

		// getters
		std::string				getName() const;
		std::string				getKey() const;
		std::string				getTopic() const;
		const clientNickMap&	getClientMap() const;
		const clientNickMap&	getOperators() const;
		// getters -> channel modes
		bool					isClientLimitMode() const;
		bool					isTopicProtectedMode() const;
		bool					isChannelProtectedMode() const;

		// setters
		void			setName(const std::string& newName);
		void			setKey(const std::string& newKey);
		void			setTopic(const std::string& newTopic);
		void			setClientLimit(const size_t& limit);
		void			setTopicProtection(const bool& status);
		void			setChannelProtection(const bool& status);
		void			addConnectedClient(const Client& clientRef);
		void			removeConnectedClient(const std::string& clientNickname);
		void			addOperator(Client& clientRef);
		void			removeOperator(const std::string& clientNickname);

		bool			checkTopic(const std::string argument);
		bool			checkName(const std::string name);
		void			broadcastNumericReplies(const size_t& numberofReplies, ...);
		std::string		getUsersList();
	
	protected:
		// add protected elements here

	private:
		clientNickMap			_connectedClients;
		clientNickMap			_operators;
		std::string				_name;
		std::string				_topic;
		std::string				_nicknameOfTopicSetter;
		std::string				_timeTopicWasSet;
		
		std::string				_key;
		// Channel modes
		size_t					_clientLimit;
		bool					_topicIsProtected;
		bool					_channelIsProtected;
		// Channel&		operator = (const Channel &copyMe);

};

#endif
