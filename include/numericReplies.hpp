#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

// 001
// #define RPL_WELCOME(server, nickname) (std::string("001 ") + server + " :Welcome to the " + server + " Network, " + nickname + "\r\n")
#define RPL_WELCOME(server, nickname) (std::string(":") + server + " 001 " + nickname + " :Welcome to the " + server + " Network, " + nickname + "\r\n")
// 002
#define RPL_YOURHOST(server, nickname, serverName, version) (std::string(":") + server + " 002 " + nickname + " :Your host is " + serverName + ", running version " + version + "\r\n")
// 003
#define RPL_CREATED(server, nickname, datetime) (std::string(":") + server + " 003 " + nickname + " :This server was created " + datetime + "\r\n")
// 004
#define RPL_MYINFO(server, nickname, serverName, version, userModes, channelModes) (std::string(":") + server + " 004 " + nickname + " :" + serverName + " " + version + " " + userModes + " " + channelModes + "\r\n")

// ...

// 333
#define RPL_TOPICWHOTIME(server, nickname, channel, nicknameOfTopicSetter, setat) (std::string(":") + server + " 333 " + nickname + " " + channel + " " + nicknameOfTopicSetter + " " + setat + "\r\n")

// 403
#define ERR_NOSUCHCHANNEL(server, nickname, channel) (std::string(":") + server + " 403 " + nickname + " " + channel + " :No such channel\r\n")
// 431
#define ERR_NONICKNAMEGIVEN(server) (std::string(":") + server + " 431 " + " :No nickname given\r\n")
// 432
#define ERR_ERRONEUSNICKNAME(server, client, nickname) (std::string(":") + server + " 432 " + client + " " + nickname + ": Erroneus nickname\r\n")
// 433
#define ERR_NICKNAMEINUSE(server, nickname) (std::string(":") + server + " 433 " + nickname + " " + nickname + " :Nickname is already in use\r\n")

// 461
#define ERR_NEEDMOREPARAMS(server, commandName) (std::string(":") + server + " 461 " + commandName + " :Not enough parameters\r\n")
// #define ERR_NEEDMOREPARAMS(server, nickname, commandName) (std::string(":") + server + " 461 " + nickname + " " + commandName + " :Not enough parameters\r\n")
// 462
#define ERR_ALREADYREGISTERED(server, nickname) (std::string(":") + server + " 462 " + nickname + " :You may not reregister.\r\n")
// 464
#define ERR_PASSWDMISMATCH(server) (std::string(":") + server + " 464" + " : Password incorrect \r\n")

// Custom Messages
#define NICK_SUCCESS(nickname1, nickname2) (std::string(":") + nickname1 + " NICK " + nickname2 + "\r\n")
#define USER_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :User created successfully!\r\n")
#define PASS_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :Password set successfully !\r\n")
#define PONG_SUCCESS(server, token) (std::string(":") + server + " PONG :" + token + "\r\n")

#define ERR_NICKCOLLISION(server, nickname) (std::string(":") + server + " 436 " + nickname + " :Nickname collision KILL \r\n")

#define KILL(server, nickname, reason ) (std::string(":") + server + " KILL " + nickname + " :" + server + "\r\n")
 
// JOIN
#define JOIN_MSG(server, channel, nickname) (std::string(":") + nickname + "!" + server + " JOIN " + channel + " " +  nickname + "\r\n")


#define JOIN_SUCCESS(server, nickname, channel) (std::string("User ") + RED + nickname + RESET + " has joined the channel " + channel + "\r\n") 
#define RPL_TOPIC(server, nickname, channel, topic) (std::string(":") + server + " 332 " + nickname + " " + channel + " :" + topic + "\r\n")
#define ERR_BADCHANMASK(channel) (std::string(":") + channel + " 476 " + ":Bad Channel Mask\r\n")
#define ERR_BADCHANNELKEY(server, nickname, channel) (std::string(":") + server +  " 475 " + nickname + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_TOOMANYCHANNELS(server, nickname, channel) (std::string(":") + server +  " 405 " + nickname + " " + channel + " :You have joined too many channels\r\n") 


// RPL_NAMREPLY (353) 
//  "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"


#define PRIVMSG(server, channel, message) (std::string(":") + server + " PRIVMSG " + channel + " :" + message)

#endif



// "<client> <nick> :Nickname is already in use"