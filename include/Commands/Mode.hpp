#ifndef MODE_HPP
# define MODE_HPP

#include "ACommand.hpp"
#include "ft_irc.hpp"
#include "Channel.hpp"

class Client;

class Mode : public ACommand
{
	public:
		typedef std::map< std::string, Channel >	channelMap;
		typedef ACommand::clientMap					clientMap;

		Mode(clientMap* clients);
		Mode(clientMap* clients, channelMap* channels);
		~Mode();

    	void		handleRequest(Client &client, std::string arg);
    	void		parseArgument();
    	void		parseArgument(std::string& arg, std::string& target, std::string& modes, std::vector<std::string>& arguments);
    	void		action();
		void		applyUserModes(Client& client, const std::string& target, std::string modes);
		void		applyChannelModes(Client& client, Channel& channel, std::string modes, std::vector<std::string>& arguments);
		bool		toggleKeyMode(Channel& channel, const char& changeMode, std::vector<std::string>& arguments, std::vector<std::string>& parametersSet);
		bool		toggleClientLimitMode(Channel& channel, const char& changeMode, std::vector<std::string>& arguments, std::vector<std::string>& parametersSet);
		bool		updateChannelOperator(Client& client, Channel& channel, const char& changeMode, std::vector<std::string>& arguments, std::vector<std::string>& parametersSet);

	protected:
		// add protected elements here

	private:
    	Mode();
		Mode(const Mode &copyMe);
		Mode&		operator = (const Mode &copyMe);

		channelMap*	_channelMap;
};

#endif