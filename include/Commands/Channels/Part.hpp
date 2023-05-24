#ifndef PART_HPP
# define PART_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "ACommand.hpp"
# include "numericReplies.hpp"
# include "ft_irc.hpp"

class Part : public ACommand
{
	public:
		Part(std::map< int, Client >* clients, std::map< std::string, Channel >* channels);
		~Part();

    	void									handleRequest(Client &client, std::string arg);
    	void									parseArgument(std::string& arg);
    	void									action(Client &client);

	private:
    	void									parseArgument();
    	void									action();

		std::map< std::string, Channel >*		_channels;
		std::vector< std::string >				_channelList;
		std::string								_reason;

		Part();
		Part(std::map< int, Client >* clients);
		Part(const Part &copyMe);
		Part&									operator = (const Part &copyMe);
};

#endif
