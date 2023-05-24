/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:40:27 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:40:28 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		Channel(const Channel &copyMe);
		Channel&				operator = (const Channel &copyMe);
		Channel();
		~Channel();
		

		// methods
		void					broadcastNumericReply(const std::string message) const;
		void					sendMessageToChannel(const std::string message, Client& client) const;
		void					sendMessageToOperators(const std::string message, Client& client) const;

		// getters
		std::string				getName() const;
		std::string				getKey() const;
		std::string				getTopic() const;
		const clientNickMap&	getClientMap() const;
		const clientNickMap&	getOperators() const;
		std::string				getNicknameOfTopicSetter() const;
		std::string				getCreationTime() const;
		std::string				getTimeTopicWasSet() const;
		size_t					getClientLimit() const;
		const nickVector&		getInvitedClients() const;
		const modeMap&			getChannelModes() const;

		// getters -> channel modes
		bool					addChannelMode(const char& mode, const std::string& parameter = "");
		bool					removeChannelMode(const char& mode);
		bool					modeIs(const char& mode) const;
		bool					modeIs(const std::string& mode) const;
		std::string				getModeParameter(const char& modeChar) const;
		std::string				getModeParameter(const std::string& modeStr) const;
		void					updateMode(const char& mode, const std::string& param);
		std::string				listModes() const;
		std::string				listModeParameters() const;

		// checkers
		bool					isClientOperator(const Client& clientRef) const;
		bool					isClientConnected(const Client& clientRef) const;
		bool					isInvited(const std::string& clientNick) const;

		// setters
		// void					setKey(const std::string& newKey);
		// void					setChannelProtection(const bool& status);
		void					setName(const std::string& newName);
		void					setTopic(const std::string& newTopic);
		// void					setClientLimit(const size_t& limit);
		// void					setTopicProtection(const bool& status);
		// void					setInviteOnly(const bool& status);
		void					addConnectedClient(const Client& clientRef, bool isChannelCreator = false);
		void					removeConnectedClient(const std::string& clientNickname);
		void					addOperator(const Client& clientRef);
		void					removeOperator(const std::string& clientNickname);

		// setters -> channel modes
		void					addInvitedClient(const std::string& clientNick);
		void					removeInvitedClient(const std::string& clientNick);

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
	
		// Channel modes
		modeMap					_channelModes;

		// Channel&		operator = (const Channel &copyMe);

};

#endif
