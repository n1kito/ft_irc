#ifndef MODE_HPP
# define MODE_HPP

#include "ACommand.hpp"
#include "ft_irc.hpp"

class Client;

class Mode : public ACommand
{
	public:
		typedef std::map< std::string, Channel >	channelMap;
		typedef ACommand::clientMap					clientMap;

		Mode(ACommand::clientMap* clients);
		Mode(ACommand::clientMap* clients, channelMap* channels);
		~Mode();

    	void		handleRequest(Client &client, std::string arg);
    	void		parseArgument();
    	// std::string	parseArgument(Client &client, std::string& arg);
    	void		action();
    	// std::string	action(Client &client, std::string nickname);

	protected:
		// add protected elements here

	private:
    	Mode();
		Mode(const Mode &copyMe);
		Mode&		operator = (const Mode &copyMe);

		channelMap*	_channelMap;
};

#endif