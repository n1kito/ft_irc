#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <sys/socket.h>
# include "Client.hpp"
# include <vector>
# include <cstdarg>
# include <sys/types.h>
# include <sys/socket.h>
# include "numericReplies.hpp"
# include "ft_irc.hpp"

class Client;

class Channel
{
	public:
		typedef std::map< std::string, const Client* >	clientNickMap;
		typedef std::vector< std::string >				nickVector;
		typedef std::map< char, std::string >			modeMap;

		Channel( std::string name, const Client& client );
		// TODO: should the copy constructors and default constructor be made private ?
		Channel(const Channel &copyMe);
		Channel&				operator = (const Channel &copyMe);
		Channel();
		~Channel();
		

		// methods
		void					broadcastNumericReply(const std::string message) const;

		// getters
		std::string				getName() const;
		std::string				getKey() const;
		std::string				getTopic() const;
		const clientNickMap&	getClientMap() const;
		const clientNickMap&	getOperators() const;
		std::string				getNicknameOfTopicSetter() const;
		std::string				getCreationTime() const;
		std::string				getTimeTopicWasSet() const;
		
		// getters -> channel modes
		bool					addChannelMode(const char& mode, const std::string& parameter = "");
		bool					removeChannelMode(const char& mode);
		bool					modeIs(const char& mode);
		bool					modeIs(const std::string& mode);
		std::string				getModeParameter(const char& modeStr);
		void					updateMode(const char& mode, const std::string& param);
		std::string				listModes() const;
		std::string				listModeParameters() const;

		// checkers
		bool					isClientOperator(const Client& clientRef) const;
		bool					isClientConnected(const Client& clientRef) const;
		bool					isInvited(const std::string& clientNick) const;

		// setters
		// void					setKey(const std::string& newKey);
		void					setChannelProtection(const bool& status);
		void					setName(const std::string& newName);
		void					setTopic(const std::string& newTopic);
		// void					setClientLimit(const size_t& limit);
		void					setTopicProtection(const bool& status);
		// void					setInviteOnly(const bool& status);
		void					addConnectedClient(const Client& clientRef);
		void					removeConnectedClient(const std::string& clientNickname);
		void					addOperator(const Client& clientRef);
		void					removeOperator(const std::string& clientNickname);

		// setters -> channel modes
		void					addInvitedClient(const std::string& clientNick);
		void					removeInvitedClient(const std::string& clientNick);

		bool					checkTopic(const std::string argument);
		bool					checkName(const std::string name);
		void					broadcastNumericReplies(const size_t numberofReplies, ...);
		std::string				getUsersList();
	
		void					setNicknameOfTopicSetter(const std::string& name);
		void					setTimeTopicWasSet(const std::string& secondsElapsedSinceUnixEpoch);

	protected:
		// add protected elements here

	private:
		clientNickMap			_connectedClients;
		nickVector				_invitedClients; // for "invite only" mode
		clientNickMap			_operators;
		std::string				_name;
		std::string				_topic;
		std::string				_nicknameOfTopicSetter;
		std::string				_timeTopicWasSet;
		std::string				_timeChannelWasCreated;
		
		std::string				_key;

		// Channel modes
		modeMap					_channelModes;
		size_t					_clientLimit;
		bool					_inviteOnly;
		bool					_topicIsProtected;
		bool					_channelIsProtected;

		// Channel&		operator = (const Channel &copyMe);

};

#endif
