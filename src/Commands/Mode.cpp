#include "Mode.hpp"

/* CONSTRUCTORS ***************************************************************/

Mode::Mode() {}
Mode::Mode(clientMap* clients) : ACommand(clients), _channelMap(NULL) {}
Mode::Mode(clientMap* clients, channelMap* channels) : ACommand(clients), _channelMap(channels) {}

Mode::Mode(const Mode &copyMe) : ACommand()
{
	// std::cout << "Copy constructor called" << std::endl;
	(void)copyMe;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Mode::~Mode()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Mode& Mode::operator = (const Mode &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Mode::parseArgument() {}
void		Mode::parseArgument(std::string& arg, std::string& target, std::string& modes, std::vector<std::string>& arguments)
{
	std::stringstream	argumentStream(arg);
	std::string			argumentsStr = "";

	// Extracting target
	argumentStream >> target;
	// Extracting modes token and adding them to modes vector
	argumentStream >> modes;
	// If stream is not empty yet, extract tokens into arguments vector
	while (argumentStream.eof() == false)
	{
		std::string token = "";
		argumentStream >> token;
		arguments.push_back(token);
	}
}

void		Mode::action() {}
void		Mode::action(Client& client, Channel& channel, std::string modes, std::vector<std::string> arguments)
{
	(void)client;
	(void)channel;
	(void)modes;
	(void)arguments;
}

void	Mode::handleRequest(Client &client, std::string arg)
{
	std::string					target;
	std::string					modes;
	std::vector<std::string>	arguments;

	(void)_channelMap;
	parseArgument(arg, target, modes, arguments);
	if (target.empty())
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "MODE").c_str());
	if (target[0] == '#')
	{
		// Target is channel
		// If the channel does not exist
		if (_channelMap->find(target) == _channelMap->end())
			sendNumericReplies(1, client.getClientSocket(), \
			ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), target).c_str());
		else
		{
			Channel*	channel = &(*_channelMap)[target];
			// If there are no modes given, return the current modes of the channel and channel creation time
			if (modes.empty())
				sendNumericReplies(2, client.getClientSocket(), \
					RPL_CHANNELMODEIS(client.getServerName(), client.getNickname(), channel->getName(), channel->getModes(), channel->getModeParameters()).c_str(),
					RPL_CREATIONTIME(client.getServerName(), client.getNickname(), channel->getName(), channel->getCreationTime()).c_str());
			// If there are modes supplied
			else
			{
				// If user cannot update modes on the channel
				if (channel->isClientOperator(client) == false)
					sendNumericReplies(1, client.getClientSocket(), \
						ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), channel->getName()).c_str());
				// Handle the requests
				else
				{
					action(client, *channel, modes, arguments);
						// else if (modeStr == "invite-only" )
						// 	mode = 'i';
						// else if (modeStr == "topic-protected")
						// 	mode = 't';
						// else if (modeStr == "key")
						// 	mode = 'k';
						// else if (modeStr == "client-limit")
						// 	mode = 'l';
				}
			}
		}
	}
	else
	{
		// Target is user
		// If the target user does not exist
		if (getClientByNickname(target) == NULL)
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_NOSUCHNICK(client.getServerName(), client.getNickname(), target).c_str());
		// If target is not the same nickname as the client who sent the command
		else if (target != client.getNickname())
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_USERSDONTMATCH(client.getServerName(), client.getNickname()).c_str());
		else if (modes.empty())
			sendNumericReplies(1, client.getClientSocket(), \
				RPL_UMODEIS(client.getServerName(), client.getNickname(), client.getUserModes()).c_str());
		else
			applyModes(client, target, modes);
	}
	// // This is useful to check what was actually parsed
	// std::cout	<< "Target: <" << target << ">" << std::endl
	// 			<< "Modes: <";
	// for (std::vector<char>::iterator it = modes.begin(); it != modes.end(); ++it)
	// 	std::cout << *it;
	// std::cout	<< ">" << std::endl
	// 			<< "Arguments: ";
	// for (std::vector<std::string>::iterator it = arguments.begin(); it != arguments.end(); ++it)
	// 	std::cout << "<" << *it << "> ";
	// std::cout << std::endl;
}

void	Mode::applyModes(Client& client, const std::string& target, std::string modes)
{
	// If the modes string does not start with + or -, do nothing
	if (modes[0] != '+' && modes[0] != '-')
		return ;
	char changeMode = modes[0];
	std::string successfullyChanged(1, changeMode);
	std::string	failedToChange = "";
	for (size_t i = 1; i < modes.length(); ++i)
	{
		if (changeMode == '+')
		{
			if (client.modeIs(modes[i]) == true)
				continue;
			if (client.addUserMode(modes[i]) == true)
				successfullyChanged.push_back(modes[i]);
			else
				failedToChange.push_back(modes[i]);
		}
		else
		{
			if (modes[i] == 'i' && client.modeIs(modes[i]) == false)
				continue;
			if (client.removeUserMode(modes[i]) == true)
				successfullyChanged.push_back(modes[i]);
			else
				failedToChange.push_back(modes[i]);
		}
	}
	if (successfullyChanged.length() > 1)
		sendNumericReplies(1, client.getClientSocket(), \
			MODE_MSG(client.getServerName(), client.getNickname(), target, successfullyChanged).c_str());
	for (size_t i = 0; i < failedToChange.length(); ++i)
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_UMODEUNKNOWNFLAG(client.getServerName(), client.getNickname(), std::string(1, failedToChange[i])).c_str());
}