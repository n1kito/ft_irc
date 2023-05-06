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

// TOPIC
// 331
#define RPL_NOTOPIC(server, nickname, channel) ( std::string(":") + server + " 331 " + nickname + " " + channel + " :No topic is set\r\n")
// 332
#define RPL_TOPIC(server, nickname, channel, topic) (std::string(":") + server + " 332 " + nickname + " " + channel + " :" + topic + "\r\n")
// 333
#define RPL_TOPICWHOTIME(server, nickname, channel, nicknameOfTopicSetter, setat) (std::string(":") + server + " 333 " + nickname + " " + channel + " " + nicknameOfTopicSetter + " " + setat + "\r\n")

// INVITE
// 341
# define RPL_INVITING(server, client, nickname, channel) (std::string(":") + server + " 341 " + client + " " + nickname + " " + channel + "\r\n")

// ...

// 401
#define ERR_NOSUCHNICK(server, client, nickname) (std::string(":") + server + " 401 " + client + " " + nickname + " :No such nick/channel\r\n")
// 403
#define ERR_NOSUCHCHANNEL(server, nickname, channel) (std::string(":") + server + " 403 " + nickname + " " + channel + " :No such channel\r\n")
// 431
#define ERR_NONICKNAMEGIVEN(server) (std::string(":") + server + " 431 " + " :No nickname given\r\n")
// 432
#define ERR_ERRONEUSNICKNAME(server, client, nickname) (std::string(":") + server + " 432 " + client + " " + nickname + ":Erroneus nickname\r\n")
// 433
#define ERR_NICKNAMEINUSE(nickname) (std::string(":433 " + nickname + " :Nickname is already in use\r\n"))
// 442
#define ERR_NOTONCHANNEL(server, client, channel) (std::string(":") + server + " 442 " + client + " " + channel + " :You're not on that channel\r\n") 
// 443
#define ERR_USERONCHANNEL(server, client, nickname, channel) (std::string(":") + server + " 443 " + client + " " + nickname + " " + channel + " :is already on channel\r\n") 
// 461
#define ERR_NEEDMOREPARAMS(server, commandName) (std::string(":") + server + " 461 " + commandName + " :Not enough parameters\r\n")
// 462
#define ERR_ALREADYREGISTERED(server, nickname) (std::string(":") + server + " 462 " + nickname + " :You may not reregister\r\n")
// 464
#define ERR_PASSWDMISMATCH(server, nickname) (std::string(":") + server + " 464 " + nickname + " :Password incorrect\r\n")
// 482
#define ERR_CHANOPRIVSNEEDED(server, client, channel) (std::string(":") + server + " 482 " + client + " " + channel + " :You're not channel operator\r\n")

// Custom Messages
#define NICK_SUCCESS(server, nickname) (std::string(":") + server + " 909 " + nickname + " :Nickname created successfully!\r\n")
#define USER_SUCCESS(server, nickname) (std::string(":") + server + " 910 " + nickname + " :User created successfully!\r\n")
#define PASS_SUCCESS(server, nickname) (std::string(":") + server + " 911 " + nickname + " :Password set successfully !\r\n")
#define PONG_SUCCESS(token) (std::string("PONG ") + token + " \r\n")

#define NICK_COLLISION(nickname) (std::string(":436 " + nickname + " :Nickname collision KILL from your_nick\r\n"))

#define KILL(nickname, reason) (std::string("KILL " + nickname + " :" + reason + "\r\n"))

// JOIN

#define JOIN_SUCCESS(nickname, channel) (std::string(":") + nickname + " JOIN " + channel + " \r\n") 

// 332 <nickname> <channel> :<topic>

#endif



// "<client> <nick> :Nickname is already in use"