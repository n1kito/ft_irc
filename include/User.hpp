#ifndef USER_HPP
# define USER_HPP

#include "ACommand.hpp"
#include "ft_irc.hpp"
#include "Client.hpp"
#include "numericReplies.hpp"


class User : public ACommand
{
	public:
		User();
		User(const User &copyMe);
		~User();
		User&		operator = (const User &copyMe);

		// void
	protected:
		std::string	getUsername() const;
		std::string	getRealname() const;
		void		setUsername( std::string username );
		void		setRealname( std::string realname );

		const char*	handleRequest( Client& client, std::string argument );
		const char*	parseArgument( std::string argument );
		const char*	action( Client& client, std::string username, std::string realname );

	private:
		std::string	_username;
		std::string	_realname;
		// add private elements here
};

#endif

/*
	Command: USER
  	Parameters: <username> <mode> * <realname>

	modes:
	a - user is flagged as away;
    i - marks a users as invisible;
    w - user receives wallops;
    r - restricted user connection;
    o - operator flag;
    O - local operator flag;
    s - marks a user for receipt of server notices.


	Numeric Replies:
	ERR_NEEDMOREPARAMS (461)
	ERR_ALREADYREGISTERED (462)

	example: 
	USER guest 0 * :Ronnie Reagan 
*/
