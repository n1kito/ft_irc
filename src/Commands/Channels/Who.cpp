#include "Who.hpp"

/* CONSTRUCTORS ***************************************************************/

Who::Who() {}
Who::Who(clientMap* clients) : ACommand(clients), _channelMap(NULL) {}
Who::Who(clientMap* clients, channelMap* channels) : ACommand(clients), _channelMap(channels) {}

Who::Who(const Who &copyMe) : ACommand()
{
	// std::cout << "Copy constructor called" << std::endl;
	(void)copyMe;
	*this = copyMe;
}

/* DESTRUCTORS ****************************************************************/

Who::~Who()
{
	// std::cout << "Destructor called" << std::endl;
}

/* OVERLOADS ******************************************************************/

Who& Who::operator = (const Who &copyMe)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	(void)copyMe;
	return *this;
}

/* METHODS ********************************************************************/

void		Who::parseArgument() {}
void		Who::parseArgument(std::string& arg, std::string& target)
{	
	std::stringstream	argStream(arg);
	argStream >> target;
}

void	Who::handleRequest(Client &client, std::string arg)
{
	std::cout << "In the handler" << std::endl;
	std::string	target;
	parseArgument(arg, target);
	std::cout << "Target is: " << target << std::endl;
	action (client, target);
}

void	Who::action() {}
void	Who::action(const Client& client, const std::string& target)
{
	std::cout << "In the action" << std::endl;
	// Target is a channel
	// Send one RPL_WHOREPLY per user connected to the target channel
	if (target.empty() == false)
	{
		if (target[0] == '#')
		{
			if (_channelMap->find(target) != _channelMap->end())
			{
				// Get the target channel
				Channel* targetChannel = &(_channelMap->at(target));
				// Retrieve the channel's client map
				const Channel::clientNickMap* targetClientMap = &targetChannel->getClientMap();
				for (Channel::clientNickMap::const_iterator it = targetClientMap->begin(); it != targetClientMap->end(); ++it)
				{			
					const Client* targetClient = it->second;
					std::cout << "Message: " << RPL_WHOREPLY(targetClient->getServerName(), targetClient->getNickname(), target, targetClient->getUsername(), targetClient->getServerName(), targetClient->getNickname(), "H", targetClient->getRealname()) << std::endl;
					sendNumericReplies(1, client.getClientSocket(),
						// RPL_WHOREPLY(targetClient->getServerName(), targetClient->getNickname(), target, targetClient->getUsername(), "783829BF.B270E442.5F584402.IP", targetClient->getNickname(), "H", targetClient->getRealname()).c_str());
						RPL_WHOREPLY(targetClient->getServerName(), targetClient->getNickname(), target, "~" + targetClient->getUsername(), "localhost", targetClient->getNickname(), "H" + (targetChannel->isClientOperator(*targetClient) ? "@" : ""), targetClient->getRealname()).c_str());
				}
			}
		}
		// Target is a user
		// Send one RPL_WHOREPLY for the specific user.
		// Channel field is literal *.
		else
		{
			// :roubaix.fr.epiknet.org 352 nikito * ~user 783829BF.B270E442.5F584402.IP roubaix.fr.epiknet.org nikito H :0 user
			// :pouetmania 352 chacha * mjallada pouetmania pouetmania chacha H :0 Matthieu JALLADAUD
			std::cout << "Message: " << RPL_WHOREPLY(client.getServerName(), client.getNickname(), "*", "~" + client.getUsername(), client.getServerName(), client.getNickname(), "H", client.getRealname()) << std::endl;
			sendNumericReplies(1, client.getClientSocket(),
				RPL_WHOREPLY(client.getServerName(), client.getNickname(), "*", "~" + client.getUsername(), client.getServerName(), client.getNickname(), "H", client.getRealname()).c_str());
		}
	}
	// :roubaix.fr.epiknet.org 315 nikito nikito :End of /WHO list.
	// :pouetmania 315 chacha chacha :End of WHO list.
	// Looks ok
	std::cout << "Message: " << RPL_ENDOFWHO(client.getServerName(), client.getNickname(), target) << std::endl;
	// Notify that the list has been fully sent
	sendNumericReplies(1, client.getClientSocket(),
		RPL_ENDOFWHO(client.getServerName(), client.getNickname(), target).c_str());
}