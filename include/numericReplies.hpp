#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

// 001
#define RPL_WELCOME(server, nickname, network) (std::string(":") + server + " 001 " + nickname + " :Welcome to the " + network + " Network, " + nickname + "\r\n")
// #define RPL_WELCOME(server, nickname, network) (std::string(":001 ") + server + " :Welcome to the " + network + " Network, " + nickname + "\r\n")
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
#define ERR_NONICKNAMEGIVEN(server) (std::string(":") + server + " 431 " + " :No nickname given\r\n")
// 432
#define ERR_ERRONEUSNICKNAME(server, client, nickname) (std::string(":") + server + " 432 " + client + " " + nickname + ":Erroneus nickname\r\n")
// 433
#define ERR_NICKNAMEINUSE(nickname) (std::string(":433 " + nickname + " :Nickname is already in use\r\n"))
// 461
#define ERR_NEEDMOREPARAMS(server, commandName) (std::string(":") + server + " 461 " + commandName + " :Not enough parameters\r\n")
// #define ERR_NEEDMOREPARAMS(server, nickname, commandName) (std::string(":") + server + " 461 " + nickname + " " + commandName + " :Not enough parameters\r\n")
// 462
#define ERR_ALREADYREGISTERED(server, nickname) (std::string(":") + server + " 462 " + nickname + " :You may not reregister\r\n")
// 464
#define ERR_PASSWDMISMATCH(server) (std::string(":") + server + " 464" + " : Password incorrect \r\n")

// Custom Messages
#define NICK_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :Nickname created successfully!\r\n")
#define USER_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :User created successfully!\r\n")
#define PASS_SUCCESS(server, nickname) (std::string(":") + server + " 911 " + nickname + " :Password set successfully !\r\n")
#define PONG_SUCCESS(server, token) (std::string("PONG : ") + server + " " + token + " \r\n")

#define NICK_COLLISION(server, nickname) (std::string(":") + server + " 436 " + nickname + " :Nickname collision KILL \r\n")

#define KILL(nickname, reason) (std::string("KILL ") + nickname + " :" + reason + "\r\n")
 
// JOIN

#define JOIN_SUCCESS(nickname, channel) (std::string(":") + nickname + " JOIN " + channel + " \r\n") 
#define RPL_TOPIC(nickname, channel, topic) (std::string(":") + nickname + " 332 " channel + " " + topic " \r\n") 

// 332 <nickname> <channel> :<topic>

#endif



// "<client> <nick> :Nickname is already in use"