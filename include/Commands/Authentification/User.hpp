#ifndef USER_HPP
# define USER_HPP

# include "ACommand.hpp"
# include "ft_irc.hpp"
# include "Client.hpp"
# include "numericReplies.hpp"
# include <limits>

class User : public ACommand
{
	public:
		User(const User &copyMe);
		User(std::map< int, Client >* clients);

		~User();
		User&		operator = (const User &copyMe);

	protected:
		std::string	getUsername() const;
		std::string	getRealname() const;
		void		setUsername(std::string username);
		void		setRealname(std::string realname);
		void		handleRequest(Client& client, std::string argument);
		void		parseArgument();
		std::string	parseArgument(Client& client, std::string argument);
		void		action();
		std::string	action(Client& client, std::string username, std::string realname); 

	private:
		User();
		std::string	_username;
		std::string	_realname;
};

#endif