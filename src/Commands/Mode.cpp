#include "Mode.hpp"

/* CONSTRUCTORS ***************************************************************/

Mode::Mode() {}
Mode::Mode(clientMap* clients) : ACommand(clients), _channelMap(NULL) {}
Mode::Mode(clientMap* clients, channelMap* channels) : ACommand(clients), _channelMap(channels) {}

Mode::Mode(const Mode &copyMe) : ACommand() { *this = copyMe; }

/* DESTRUCTORS ****************************************************************/

Mode::~Mode() {}

/* OVERLOADS ******************************************************************/

Mode& Mode::operator = (const Mode &copyMe)
{
	(void)copyMe;
	return *this;
} 

/* METHODS ********************************************************************/

void		Mode::parseArgument() {}
void		Mode::parseArgument(std::string& arg, std::string& target, std::string& modes, std::vector<std::string>& arguments)
{
	std::stringstream	argumentStream(arg);
	std::string			argumentsStr = "";

	argumentStream >> target;
	argumentStream >> modes;
	while (argumentStream.eof() == false)
	{
		std::string token = "";
		argumentStream >> token;
		arguments.push_back(token);
	}
}

void	Mode::action() {}

void	Mode::handleRequest(Client &client, std::string arg)
{ 
	std::string					target;
	std::string					modes;
	std::vector<std::string>	arguments;

	parseArgument(arg, target, modes, arguments);
	if (target.empty())
		sendNumericReplies(1, client.getClientSocket(), \
			ERR_NEEDMOREPARAMS(client.getServerName(), client.getNickname(), "MODE").c_str());
	if (modes.length() > MODES + 1)
		modes = modes.substr(0, MODES + 1);
	if (target[0] == '#')
	{
		if (_channelMap->find(target) == _channelMap->end())
			sendNumericReplies(1, client.getClientSocket(), \
			ERR_NOSUCHCHANNEL(client.getServerName(), client.getNickname(), target).c_str());
		else
		{
			Channel*	channel = &(*_channelMap)[target];
			if (modes.empty())
				sendNumericReplies(2, client.getClientSocket(), \
					RPL_CHANNELMODEIS(client.getServerName(), client.getNickname(), channel->getName(), channel->listModes(), channel->listModeParameters()).c_str(),
					RPL_CREATIONTIME(client.getServerName(), client.getNickname(), channel->getName(), channel->getCreationTime()).c_str());
			else if (modes == "b")
				sendNumericReplies(1, client.getClientSocket(), \
					std::string(":pouetmania 368 " + client.getNickname() + " " + target + " :End of channel ban list\r\n").c_str());
			else
			{
				if (channel->isClientOperator(client) == false)
					sendNumericReplies(1, client.getClientSocket(), \
						ERR_CHANOPRIVSNEEDED(client.getServerName(), client.getNickname(), channel->getName()).c_str());
				else if (modes[0] != '+' && modes[0] != '-')
					return;
				else
					applyChannelModes(client, *channel, modes, arguments);
			}
		}
	}
	else
	{
		if (getClientByNickname(target) == NULL)
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_NOSUCHNICK(client.getServerName(), client.getNickname(), target).c_str());
		else if (target != client.getNickname())
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_USERSDONTMATCH(client.getServerName(), client.getNickname()).c_str());
		else if (modes.empty())
			sendNumericReplies(1, client.getClientSocket(), \
				RPL_UMODEIS(client.getServerName(), client.getNickname(), client.getUserModes()).c_str());
		else if (modes[0] != '+' && modes[0] != '-')
			return;
		else
			applyUserModes(client, target, modes);
	}
}

void	Mode::applyUserModes(Client& client, const std::string& target, std::string modes)
{
	char 		changeMode = modes[0];
	std::string successfullyChanged(1, changeMode);
	std::string	parametersSet;

	for (size_t i = 1; i < modes.length(); ++i)
	{
		if (changeMode == '+')
		{
			if (client.modeIs(modes[i]) == true)
				continue;
			if (client.addUserMode(modes[i]) == true)
				successfullyChanged.push_back(modes[i]);
			else
				sendNumericReplies(1, client.getClientSocket(), \
					ERR_UMODEUNKNOWNFLAG(client.getServerName(), client.getNickname(), std::string(1,  modes[i])).c_str());
		}
		else if (changeMode == '-')
		{
			if (modes[i] == 'i' && client.modeIs(modes[i]) == false)
				continue;
			if (client.removeUserMode(modes[i]) == true)
				successfullyChanged.push_back(modes[i]);
			else
				sendNumericReplies(1, client.getClientSocket(), \
					ERR_UMODEUNKNOWNFLAG(client.getServerName(), client.getNickname(), std::string(1, modes[i])).c_str());
		}
	}
	if (successfullyChanged.length() > 1)
		sendNumericReplies(1, client.getClientSocket(), \
			MODE_MSG(client.getServerName(), client.getNickname(), client.getUsername(), target, successfullyChanged).c_str());
}

void		Mode::applyChannelModes(Client& client, Channel& channel, std::string modes, std::vector<std::string>& arguments)
{
	char 						changeMode = modes[0];
	std::string					successfullyChanged(1, changeMode);
	std::vector<std::string>	changedParameters;

	for (size_t modesIndex = 1; modesIndex < modes.length(); ++modesIndex)
	{
		if (successfullyChanged.find(modes[modesIndex], 0) != std::string::npos)
			continue;		else if (modes[modesIndex] == 'i' || modes[modesIndex] == 't')
		{
			if (changeMode == '+' && channel.modeIs(modes[modesIndex]) == false)
			{
				channel.addChannelMode(modes[modesIndex]);
				successfullyChanged.push_back(modes[modesIndex]);
			}
			else if (changeMode == '-' && channel.modeIs(modes[modesIndex]) == true)
			{
				channel.removeChannelMode(modes[modesIndex]);
				successfullyChanged.push_back(modes[modesIndex]);
			}
		}
		else if (modes[modesIndex] == 'k')
		{
			if (toggleKeyMode(channel, changeMode, arguments, changedParameters) == true)
				successfullyChanged += "k";
		}
		else if (modes[modesIndex] == 'l')
		{
			if (toggleClientLimitMode(client, channel, changeMode, arguments, changedParameters) == true)
				successfullyChanged += "l";
		}
		else if (modes[modesIndex] == 'o')
		{
			if (updateChannelOperator(client, channel, changeMode, arguments, changedParameters) == true)
				successfullyChanged += 'o';
		}
		else
			sendNumericReplies(1, client.getClientSocket(), \
				ERR_UNKNOWNMODE(client.getServerName(), client.getNickname(), std::string(1, modes[modesIndex])).c_str());
	}
	std::string changedParametersStr = "";
	if (changedParameters.empty() == false)
	{
		for (std::vector<std::string>::iterator it = changedParameters.begin(); it != changedParameters.end(); ++it)
		{
			changedParametersStr += *it;
			if (it != --changedParameters.end())
				changedParametersStr += " ";
		}
		successfullyChanged += " " + changedParametersStr;
	}
	if (successfullyChanged.length() > 1)
		channel.broadcastNumericReply(MODE_MSG(client.getServerName(), client.getNickname(), client.getUsername(), channel.getName(), successfullyChanged).c_str());
}

bool	Mode::toggleKeyMode(Channel& channel, const char& changeMode, std::vector<std::string>& arguments, std::vector<std::string>& parametersSet)
{
	if (changeMode == '+')
	{
		if (arguments.size())
		{
			std::string	password(arguments[0]);
			channel.updateMode('k', password);
			parametersSet.push_back(password);
			arguments.erase(arguments.begin());
			return true;
		}
	}
	else if (changeMode == '-' && channel.modeIs('k') == true)
	{
		channel.removeChannelMode('k');
		return true;
	}
	return false;
}

bool	Mode::toggleClientLimitMode(Client& client, Channel& channel, const char& changeMode, std::vector<std::string>& arguments, std::vector<std::string>& parametersSet)
{
	if (changeMode == '+')
	{
		if (arguments.size())
		{
			size_t				clientLimit;
			std::stringstream	parameter(arguments[0]);
			parameter >> clientLimit;
			if (parameter.fail())
			{
				arguments.erase(arguments.begin());
				return false;
			}
			else if (clientLimit == 0 || clientLimit > MAXCLIENTS)
			{
				arguments.erase(arguments.begin());
				if (clientLimit > MAXCLIENTS)
					sendNumericReplies(1, client.getClientSocket(), \
						CSTM_MAXCLIENTS(client.getServerName(), client.getNickname()).c_str());
				return false;
			}
			channel.updateMode('l', arguments[0]);
			parametersSet.push_back(arguments[0]);
			arguments.erase(arguments.begin());
			return true;
		}
	}
	else if (changeMode == '-' && channel.modeIs('l') == true)
	{
		channel.removeChannelMode('l');	
		return true;
	}
	return false;
}

bool	Mode::updateChannelOperator(Client& client, Channel& channel, const char& changeMode, std::vector<std::string>& arguments, std::vector<std::string>& parametersSet)
{
	std::vector< std::string >	usernames;
	std::string					tmpToken;
	std::stringstream			parametersStream(arguments.empty() ? "" : arguments[0]);
	bool						updatedOperators = false;

	while (std::getline(parametersStream, tmpToken, ','))
		usernames.push_back(tmpToken);
	for (std::vector<std::string>::iterator it = usernames.begin(); it != usernames.end(); ++it)
	{
		Client*	targetClient = getClientByNickname(*it);
		if (targetClient == NULL)
			sendNumericReplies(1, client.getClientSocket(), \
					ERR_NOSUCHNICK(client.getServerName(), client.getNickname(), arguments[0]).c_str());
		else if (channel.isClientConnected(*targetClient) == false)
			sendNumericReplies(1, client.getClientSocket(), \
					ERR_USERNOTINCHANNEL(client.getServerName(), client.getNickname(), targetClient->getNickname(), channel.getName()).c_str());
		else if (changeMode == '+')
		{
			if (channel.isClientOperator(*targetClient) == false)
			{
				channel.addOperator(*targetClient);
				parametersSet.push_back(*it);
				updatedOperators = true;
			}
		}
		else if (changeMode == '-')
		{
			if (channel.isClientOperator(*targetClient))
			{
				channel.removeOperator((*targetClient).getNickname());
				parametersSet.push_back(*it);
				updatedOperators = true;
			}
		}
	}
	if (arguments.empty() == false)
		arguments.erase(arguments.begin());
	return updatedOperators;
}
