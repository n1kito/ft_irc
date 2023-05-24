#ifndef QUIT_HPP
# define QUIT_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "ACommand.hpp"
# include "numericReplies.hpp"
# include "ft_irc.hpp"

class Quit : public ACommand
{
	public:
		typedef std::map< std::string, Channel > channelMap;
		
		Quit(ACommand::clientMap* clients, channelMap* channels);
		~Quit();

		void			handleRequest(Client &client, std::string arg);
		void			parseArgument(Client& client, std::string& arg);
    	void			action(Client &client, std::string& arg);

	private:
	    void			parseArgument();
    	void			action();

		channelMap*		_channelMap;

		Quit();
		Quit(const Quit &copyMe);
		Quit(std::map< int, Client >* clients);
		Quit&			operator = (const Quit &copyMe);
};

#endif
