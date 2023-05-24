/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgosseli, jeepark, mjallada                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:39:54 by pouetTeam         #+#    #+#             */
/*   Updated: 2023/05/24 13:39:55 by pouetTeam        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void	checkArguments(const int& argc, char** arguments, int& port, std::string& password)
{
	if (argc != 3)
		throw std::invalid_argument("wrong argument count. Usage: ./ircserv <PORT> <PASSWORD>");
	std::string			portStr(arguments[1]);
	if (portStr.length() > 5)
		throw std::invalid_argument("port value is too large");
	for (size_t i = 0; arguments[1][i]; ++i)
		if (!isdigit(static_cast<int>(arguments[1][i])))
			throw std::invalid_argument("port number can only cotnain digits");
	port = -1;
	std::stringstream	argumentStream;
	argumentStream << arguments[1];
	argumentStream >> port;
	if (port == -1 || port > 65535)
		throw std::invalid_argument("invalid port value");
	password = arguments[2];
	// Check that password is not too short
	if (password.size() < 12 || password.size() > 20)
		throw std::invalid_argument("password has to be between 12 and 20 characters");
	bool	containsLowercase = false;
	bool	containsUppercase = false;
	bool	containsDigits = false;
	for (size_t i = 0; i < password.length(); ++i)
	{
		if (password[i] == ' ' || password[i] == '\t')
			throw std::invalid_argument("password contains whitespaces");
		else if (isupper(static_cast<int>(password[i])))
			containsUppercase = true;
		else if (islower(static_cast<int>(password[i])))
			containsLowercase = true;
		else if (isdigit(static_cast<int>(password[i])))
			containsDigits = true;
		else
			throw std::invalid_argument("password contains forbidden characters. Allowed: [a-z], [A-Z], [0-9]");
	}
	if (containsLowercase == false)
		throw std::invalid_argument("password must contain at least one lowercase letter");
	else if (containsUppercase == false)
		throw std::invalid_argument("password must contain at least one uppercase letter");
	else if (containsDigits == false)
		throw std::invalid_argument("password must contain at least one digit");
}

// Send several numeric replies to one client
void	sendNumericReplies(const size_t& numberOfReplies, const size_t clientFd, ...)
{
	std::va_list	messages;

	va_start(messages, clientFd);
	if (numberOfReplies == 0)
		return;
	for (size_t i = 0; i < numberOfReplies; ++i)
	{
		std::string	message(va_arg(messages, char*));
		send(clientFd, message.c_str(), message.length(), 0);
	}
	va_end(messages);
}

// Returns a human readable string of the current date
std::string	getCurrentDate()
{
 	std::time_t time = std::time(0);
	std::stringstream	timeStream;
	timeStream << time;
	return timeStream.str();
}

// Custom numeric replies
void	sendCustomNumericReply(const std::string& message, const int& code, const Client& client)
{
	std::stringstream messageStream;
	messageStream << ":" << client.getServerName() << " " << code << " " << client.getNickname() << " :" << message << "\r\n";
	std::string returnMessage = messageStream.str();
	sendNumericReplies(1, client.getClientSocket(), returnMessage.c_str());
}

// Trim functions
void	leftTrim(std::string& str, std::string trimmer)
{
	size_t pos = str.find_first_not_of(trimmer);
	str.erase(0, pos);
}

void	rightTrim(std::string& str, std::string trimmer)
{
	size_t pos = str.find_first_of(trimmer);
	str.erase(pos);
}

void	rlTrim(std::string& str, std::string trimmer)
{
	leftTrim(str, trimmer);
	rightTrim(str, trimmer);
}

// Send welcome message
void	sendWelcomeMessage(const Client& client)
{
	sendCustomNumericReply(" ", 999, client);
	sendCustomNumericReply("  ░█░█░█▀▀░█░░░█▀▀░█▀█░█▄█░█▀▀░", 999, client);
	sendCustomNumericReply("  ░█▄█░█▀▀░█░░░█░░░█░█░█░█░█▀▀░", 999, client);
	sendCustomNumericReply("  ░▀░▀░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░▀░▀▀▀░", 999, client);
	sendCustomNumericReply("  ░▀█▀░█▀█░", 999, client);
	sendCustomNumericReply("  ░░█░░█░█░", 999, client);
	sendCustomNumericReply("  ░░▀░░▀▀▀░", 999, client);
	sendCustomNumericReply("  ░▀█▀░█░█░█▀▀░", 999, client);
	sendCustomNumericReply("  ░░█░░█▀█░█▀▀░", 999, client);
	sendCustomNumericReply("  ░░▀░░▀░▀░▀▀▀░", 999, client);
	sendCustomNumericReply("  ░█▀█░█▀█░█░█░█▀▀░▀█▀░", 999, client);
	sendCustomNumericReply("  ░█▀▀░█░█░█░█░█▀▀░░█░░ 💜", 999, client);
	sendCustomNumericReply("  ░▀░░░▀▀▀░▀▀▀░▀▀▀░░▀░░", 999, client);
	sendCustomNumericReply(" ", 999, client);
}

// Output users and channels
void	outputUsersChannels(std::map<int, Client>& clients, std::map<std::string, Channel>& channels)
{
	int i = 0;
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it == clients.begin())
			std::cout << UNDERLINE << "Connected clients:" << RESET << " ";
		std::cout << "[" << DIM << i++ << RESET << " " << it->second.getNickname() << "]" << ((it != --clients.end()) ? ", " : "\n");
	}
	std::cout << std::endl;
	i = 0;
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it == channels.begin())
			std::cout << UNDERLINE << "Available channels:" << RESET << " ";
		std::cout << "[" << DIM << i++ << RESET << " " << it->second.getName() << "]" << ((it != --channels.end()) ? ", " : "\n");
	}
}

void	printServerTitle()
{
	std::cout << std::endl;
	std::cout << "░█░░░█▀▀░░░█▀█░█▀█░█░█░█▀▀░▀█▀░░░█▀▀░█▀▀░█▀▄░█░█░█▀▀░█▀▄░" << std::endl;
	std::cout << "░█░░░█▀▀░░░█▀▀░█░█░█░█░█▀▀░░█░░░░▀▀█░█▀▀░█▀▄░▀▄▀░█▀▀░█▀▄░" << std::endl;
	std::cout << "░▀▀▀░▀▀▀░░░▀░░░▀▀▀░▀▀▀░▀▀▀░░▀░░░░▀▀▀░▀▀▀░▀░▀░░▀░░▀▀▀░▀░▀░" << std::endl;
	std::cout << std::endl;
}

void 	signalHandler(int signal)
{
    if (signal == SIGINT) 
		g_running = 0;
}