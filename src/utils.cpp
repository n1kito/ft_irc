#include "ft_irc.hpp"

//TODO test this
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
	// TODO is this check enough for cases where user passes port like 8080a ?
	if (port == -1 || port > 65535)
		throw std::invalid_argument("invalid port value");
	argumentStream << arguments[2];
	argumentStream >> password;
	// Check that password is not too short
	if (password.length() < 12)
		throw std::invalid_argument("password too short");
	bool	containsLowercase = false;
	bool	containsUppercase = false;
	bool	containsDigits = false;
	bool	containsSpecialChar = false;
	for (size_t i = 0; i < password.length(); ++i)
	{
		if (password[i] == ' ' || password[i] == '\t')
			throw std::invalid_argument("password contains whitespaces");
		else if (isupper(static_cast<int>(password[i])))
			containsUppercase = false;
		else if (islower(static_cast<int>(password[i])))
			containsLowercase = true;
		else if (isdigit(static_cast<int>(password[i])))
			containsDigits = true;
		else if (isSpecialCharacter(password[i]))
			containsSpecialChar = true;
		else
			throw std::invalid_argument("password contains forbidden characters. Allowed: [a-z], [A-Z], [! - #]");
	}
	if (containsLowercase == false)
		throw std::invalid_argument("password must contain at least one lowercase letter");
	else if (containsUppercase == false)
		throw std::invalid_argument("password must contain at least one uppercase letter");
	else if (containsDigits == false)
		throw std::invalid_argument("password must contain at least one digit");
	else if (containsSpecialChar == false)
		throw std::invalid_argument("password must contain at least one special character. Allowed: [! - #] ");
}

// Not really needed in a separate function
// but useful if we want to accept more special characters later on.
bool	isSpecialCharacter(char checkMe)
{
	return (checkMe == '!' || checkMe == '-' || checkMe == '#');
}

// Send several numeric replies to one client
void	sendNumericReplies(const size_t& numberOfReplies, const size_t& clientFd, ...)
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
	time_t		rawTime;
	struct tm*	timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);
	std::string	returnValue(asctime(timeInfo));
	return returnValue;
}

// Custom numeric replies
void	sendCustomNumericReply(const std::string& message, const int& code, const Client& client)
{
	std::stringstream messageStream;
	messageStream << ":" << client.getServerName() << " " << code << " " << client.getNickname() << " :" << message << "\r\n";
	std::string returnMessage = messageStream.str();
	send(client.getClientSocket(), returnMessage.c_str(), returnMessage.size(), 0);
}