#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <sys/socket.h>
# include "Client.hpp"
# include <vector>

class Client;

class Channel
{
	public:
		typedef std::map< std::string, const Client* >	clientNickMap;
		typedef std::vector< std::string >				nickVector;

		Channel( std::string name, const Client& client );
		Channel(const Channel &copyMe);
		~Channel();
		

		// methods
		void					broadcastNumericReply(const std::string message) const;

		// getters
		std::string				getName() const;
		std::string				getTopic() const;
		const clientNickMap&	getClientMap() const;
		const clientNickMap&	getOperators() const;
		std::string				getNicknameOfTopicSetter() const;
		std::string				getTimeTopicWasSet() const;
		
		// getters -> channel modes
		bool					isClientLimitMode() const;
		bool					isTopicProtectedMode() const;
		bool					isInviteOnly() const;

		// checkers
		bool					isClientOperator(const Client& clientRef) const;
		bool					isClientConnected(const Client& clientRef) const;
		bool					isInvited(const std::string& clientNick) const;

		// setters
		void					setName(const std::string& newName);
		void					setTopic(const std::string& newTopic);
		void					setClientLimit(const size_t& limit);
		void					setTopicProtection(const bool& status);
		void					setInviteOnly(const bool& status);
		void					addConnectedClient(const Client& clientRef);
		void					removeConnectedClient(const std::string& clientNickname);
		void					addOperator(Client& clientRef);
		void					removeOperator(const std::string& clientNickname);
		void					setNicknameOfTopicSetter(const std::string& name);
		void					setTimeTopicWasSet(const std::string& time);

		// setters -> channel modes
		void					addInvitedClient(const std::string& clientNick);
		void					removeInvitedClient(const std::string& clientNick);

		bool					checkTopic(const std::string argument);
		bool					checkName(const std::string name);

	protected:
		// add protected elements here

	private:
		Channel();
		clientNickMap			_connectedClients;
		nickVector				_invitedClients; // for "invite only" mode
		clientNickMap			_operators;
		std::string				_name;
		std::string				_topic;
		std::string				_nicknameOfTopicSetter;
		std::string				_timeTopicWasSet;
		
		// Channel modes
		size_t					_clientLimit;
		bool					_inviteOnly;
		bool					_topicIsProtected;

		Channel&		operator = (const Channel &copyMe);

};

#endif
