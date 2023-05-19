#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <unistd.h>
# include <exception>
# include <stdexcept>
# include <iostream> // TODO: added this to debug, not necessary
# include "Channel.hpp"

class Channel;

class Client
{
	public:
		typedef std::map< std::string, Channel* > channelsMap;
		Client();
		Client(const int& clientSocket, const std::string& serverName);
		~Client();
		Client(const Client &copyMe);
	
		Client&			operator = (const Client &copyMe);

		bool			getRegisterState() const;
		std::string		getUsername() const;
		std::string		getRealname() const;
		std::string		getNickname() const;
		std::string		getPassword() const;
		int				getClientSocket() const;
		bool			getWelcomeState() const;
		std::string		getServerName() const;
		bool			getPasswordStatus() const;
		const channelsMap&	getChannelsMap() const;
	
		std::string		getUserModes() const;

		// getters -> channel modes
		bool			addUserMode(const char& mode);
		bool			removeUserMode(const char& mode);
		bool			modeIs(const char &mode);

		void			setRegisterState(bool state);
		void			setUsername(std::string username);
		void			setRealname(std::string realname);
		void			setNickname(std::string nickname);
		void			setPassword(std::string password);
		void			setWelcomeState(const bool& state);
		
		bool			isAuthentificated() const;
		void			setPasswordStatus(const bool& status);
		void			setInvitationStatus(const bool& status);
		void			addChannel(Channel& channelRef);
		void			removeChannel(const std::string& channelName);
		void			leaveAllChannels();
		void			QuitServer(const std::string& message, std::map< std::string, Channel >* channelsMap);

	protected:
		// add protected elements here

	private:
		channelsMap		_connectedToChannels;
		bool			_isRegistered;
		bool			_passwordStatus;
		bool			_invitationStatus;
		std::string		_username;
		std::string		_realname;
		std::string		_nickname;
		std::string		_password;
		int				_clientSocket;
		bool			_welcomeState;

		std::string		_userModes;

		std::string		_serverName;
};

#endif
