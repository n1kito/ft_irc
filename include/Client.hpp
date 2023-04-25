#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client
{
	public:
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
		std::string		getServerName() const;
		bool			getPasswordStatus() const;

		void			setRegisterState(bool state);
		void			setUsername(std::string username);
		void			setRealname(std::string realname);
		void			setNickname(std::string nickname);
		void			setPassword(std::string password);
		void			setPasswordStatus(const bool& status);

	protected:
		// add protected elements here

	private:
		bool		_isRegistered;
		bool		_passwordStatus;
		std::string	_username;
		std::string	_realname;
		std::string	_nickname;
		std::string	_password;
		int			_clientSocket;
		std::string	_serverName;
};

#endif
