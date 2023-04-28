#ifndef JOIN_HPP
# define JOIN_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "ACommand.hpp"
# include "numericReplies.hpp"

class Join : public ACommand
{
	public:
		Join(std::map< int, Client >* clients, std::map< std::string, Channel >* channels);
		~Join();

    	void		handleRequest(Client &client, std::string arg);
    	std::string	parseArgument(Client &client, std::string& arg);
    	void		action(Client &client);
		void		createChannel(Client& client, std::string channelName);

	protected:
		// add protected elements here

	private:
		// add private elements here
		std::map< std::string, Channel >*		_channels;
		std::vector< std::string >				_channelsToJoin;
		
    	void		parseArgument();
    	void		action();
		Join();
		Join(std::map< int, Client >* clients);
		Join(const Join &copyMe);
		Join&		operator = (const Join &copyMe);

};

#endif
