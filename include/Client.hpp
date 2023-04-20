#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client
{
	public:
		Client();
		~Client();
		Client(const Client &copyMe);
	
		Client&		operator = (const Client &copyMe);
		bool		_isRegistered;
		std::string	_username;
		std::string	_nickname;
		std::string	_password;
	protected:
		// add protected elements here

	private:

};

#endif
