#ifndef PASS_HPP
# define PASS_HPP

# include "ft_irc.hpp"
# include "Client.hpp"
# include <string>
# include <map>
# include "ACommand.hpp"
# include "numericReplies.hpp"

class Pass : public ACommand
{
	public:
		typedef std::map< int, Client >	clientMap;

		Pass(clientMap* clients);
		Pass(clientMap* clients, const std::string& serverPassword);
		~Pass();

		void 		parseArgument();
		void 		action();
		void		handleRequest(Client& client, std::string argument);

	private:
		Pass();
		Pass(const Pass &copyMe);
		Pass&		operator = (const Pass &copyMe);

		std::string _serverPassword;
};

#endif
