#ifndef FT_IRC_HPP
#define FT_IRC_HPP

/*
░█▀▀░█▀█░█░░░█▀█░█▀▄░█▀▀
░█░░░█░█░█░░░█░█░█▀▄░▀▀█
░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░▀░▀▀▀
*/

#define BLACK "\033[30m"
#define RED "\033[31m"
#define BRED "\033[1;31m"
#define GREEN "\033[32m"
#define BGREEN "\033[1;32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define FADED_ORANGE "\033[48;2;239;123;81;30m"
#define LIGHT_BLUE "\033[38;2;46;101;124m"
#define LIGHT_GRAY "\033[38;2;192;192;192;30m"
#define LIGHT_GRAY_BLOC "\033[48;2;192;192;192;30m"
#define WHITE_BLOC "\033[30;47m"
#define YELLOW_BLOC "\033[30;43m"
#define RED_BLOC "\033[30;41m"
#define ITALIC "\033[3m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define DIM "\033[2m"
#define HIGHLIGHT "\033[7;37m"
#define RESET "\033[0m"

#define SEPARATOR std::cout << std::endl << DIM << "****************************************" << RESET << std::endl << std::endl
#define PRINT(x, y) std::cout << x << ": " << y << std::endl

// TODO: check that these are all used
// SERVER PARAMETERS
#define REALNAMELEN 50 // not part of RPL_ISUPPORT REPLY
// Maximum length of a username
#define USERLEN 10
// Max length of the nickname a user can set
#define NICKLEN 10
// Max name of channel name
#define CHANNELLEN 10
// Number of channels a client can join
#define CHANLIMIT 20
//
#define MAXCLIENTS 100
// Maximum length of the reason of the KICK command. May not be applied.
#define KICKLEN 255
// Maximum number of modes that can be specified in one MODE command
#define MODES 50
// Max length of topic a user can set on a channel
// 307 is the default value for most servers
#define TOPICLEN 307
// Types of channel available
#define CHANTYPES "#"
// Modes of channel available
#define CHANMODES ",k,l,ti"
// Prefixes available for channel members
#define PREFIX "(o)@"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <cctype>
#include <cstdarg>
#include <ctime>
#include <csignal>

#include <algorithm>
// #include "ACommand.hpp"
// #include "Nick.hpp"
#include "Client.hpp"
#include "Channel.hpp"
class Client;
class Channel;

extern int	g_running;

// Prototypes
void 		checkArguments(const int &argc, char **arguments, int &port, std::string &password);
bool 		isSpecialCharacter(char checkMe);
void 		sendNumericReplies(const size_t &numberOfReplies, const size_t clientFd, ...);
void 		sendCustomNumericReply(const std::string &message, const int &code, const Client &client);
std::string getCurrentDate();
void		leftTrim(std::string& str, std::string trimmer);
void		rightTrim(std::string& str, std::string trimmer);
void		rlTrim(std::string& str, std::string trimmer);
void		sendWelcomeMessage(const Client& client);
void		outputUsersChannels(std::map<int, Client>& clients, std::map<std::string, Channel>& channels);
void		printServerTitle();
void		signalHandler(int signal);
#endif // FT_IRC_HPP