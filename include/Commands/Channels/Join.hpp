#ifndef JOIN_HPP
# define JOIN_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "ACommand.hpp"
# include "numericReplies.hpp"
# include "ft_irc.hpp"

class Join : public ACommand
{
	public:
		Join(std::map< int, Client >* clients, std::map< std::string, Channel >* channels);
		~Join();

    	void		handleRequest(Client &client, std::string arg);
    	std::string	parseArgument(std::string& arg);
    	std::string action(Client &client);

		std::string createErrorTooManyChannels(Client const &client, size_t idx);

	protected:
		// add protected elements here

	private:
		// existing channels from Server
		std::map< std::string, Channel >*		_channels;
		// channels to create or join with keys
		std::vector< std::string >				_channelList;
		std::vector< std::string >				_keyList;
		
    	void		parseArgument();
    	void		action();
		Join();
		Join(std::map< int, Client >* clients);
		Join(const Join &copyMe);
		Join&		operator = (const Join &copyMe);

};

#endif
