#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client
{
	public:
		Client();
		Client( int clientSocket );
		~Client();
		Client(const Client &copyMe);
	
		Client&			operator = (const Client &copyMe);

		bool			getRegisterState() const;
		std::string		getUsername() const;
		std::string		getRealname() const;
		std::string		getNickname() const;
		std::string		getPassword() const;
		int				getClientSocket() const;

		void			setRegisterState(bool state);
		void			setUsername(std::string username);
		void			setRealname(std::string realname);
		void			setNickname(std::string nickname);
		void			setPassword(std::string password);

	protected:
		// add protected elements here

	private:
		bool		_isRegistered;
		std::string	_username;
		std::string	_realname;
		std::string	_nickname;
		std::string	_password;
		int	_clientSocket;

};

#endif
