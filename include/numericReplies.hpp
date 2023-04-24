#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP

// 001
# define RPL_WELCOME(server, nickname, network) (std::string(":") + server + " 001 " + nickname + ":Welcome to the " + network + " Network, " + nickname + "\r\n")
// 002
# define RPL_YOURHOST(server, nickname, serverName, ersion) (std::string(":") + server + " 002 " + nickname + ":Your host is " + serverName + ", running version " + version).c_str()
// 003
# define RPL_CREATED(server, nickname, datetime) (std::string(":") + server + " 003 " + nickname + ":This server was created " + datetime).c_str()
// 004
# define RPL_MYINFO(server, nickname, serverName, version, userModes, channelModes) (std::string(":") + server + " 004 " + nickname + ":" + serverName + " " + version + " " + userModes + " " + channelModes).c_str()

// ...

// 403
# define ERR_NOSUCHCHANNEL(server, nickname, channel) (std::string(":") + server + " 403 " + nickname + " " + channel + " :No such channel\r\n").c_str()
// 461
# define ERR_NEEDMOREPARAMS(server, nickname, commandName) (std::string(":") + server + " 461 " + nickname + " " + commandName + " :Not enough parameters\r\n").c_str()
// 462
# define ERR_ALREADYREGISTRED(server, nickname) (std::string(":") + server + " 462 " + nickname + ":You may not reregister\r\n" ).c_str()

// NICK
// 431
// #define ERR_NONICKNAMEGIVEN(server) (std::string(":") + server + " 431 " + ":No nickname given\r\n")
#define ERR_NONICKNAMEGIVEN ("test\n")

// #define ERR_ERRONEUSNICKNAME (<client> <nick> :Erroneus nickname)
#endif