#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

// 001
#define RPL_WELCOME(server, nickname, network) (std::string(":") + server + " 001 " + nickname + " :Welcome to the " + network + " Network, " + nickname + "\r\n")
// 002
#define RPL_YOURHOST(server, nickname, serverName, ersion) (std::string(":") + server + " 002 " + nickname + " :Your host is " + serverName + ", running version " + version + "\r\n")
// 003
#define RPL_CREATED(server, nickname, datetime) (std::string(":") + server + " 003 " + nickname + " :This server was created " + datetime + "\r\n")
// 004
#define RPL_MYINFO(server, nickname, serverName, version, userModes, channelModes) (std::string(":") + server + " 004 " + nickname + " :" + serverName + " " + version + " " + userModes + " " + channelModes + "\r\n")

// ...

// 403
#define ERR_NOSUCHCHANNEL(server, nickname, channel) (std::string(":") + server + " 403 " + nickname + " " + channel + " :No such channel\r\n")
// 431
#define ERR_NONICKNAMEGIVEN(nickname) (std::string(":") + nickname + " 431 " + ":No nickname given\r\n")
// 432
#define ERR_ERRONEUSNICKNAME(client, nickname) (std::string(":") + client + " " + nickname + " 432 " + ":Erroneus nickname\r\n")
// 436
#define ERR_NICKCOLLISION(client, nickname, username, hostname) (std::string(":") + server + " " + client + " 436 " + nickname + " :Nickname collision KILL from <" + user + ">@<" + hostname + ">\r\n")
// 461
#define ERR_NEEDMOREPARAMS(server, nickname, commandName) (std::string(":") + server + " 461 " + nickname + " " + commandName + " :Not enough parameters\r\n")
// 462
#define ERR_ALREADYREGISTERED(server, nickname) (std::string(":") + server + " 462 " + nickname + " :You may not reregister\r\n")

// Custom Messages
#define NICK_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :Nickname created successfully\r\n")

#define USER_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :User created successfully!\r\n")

#endif