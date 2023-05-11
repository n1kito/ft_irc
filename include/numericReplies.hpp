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

// TOPIC
// 331
#define RPL_NOTOPIC(server, nickname, channel) ( std::string(":") + server + " 331 " + nickname + " " + channel + " :No topic is set\r\n")
// 332
#define RPL_TOPIC(server, nickname, channel, topic) (std::string(":") + server + " 332 " + nickname + " " + channel + " :" + topic + "\r\n")
// 333
#define RPL_TOPICWHOTIME(server, nickname, channel, nicknameOfTopicSetter, setat) (std::string(":") + server + " 333 " + nickname + " " + channel + " " + nicknameOfTopicSetter + " " + setat + "\r\n")

// 353
#define RPL_NAMREPLY(server, nickname, channel, listUsers) (std::string(":") + server + " 353 " + nickname + " = " + channel + " :" + listUsers + "\r\n")

// 366 
#define RPL_ENDOFNAMES(server, nickname, channel) (std::string(":") + server + " 366 " + nickname + " " + channel + " :End of /NAMES list\r\n")

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
#define ERR_ERRONEUSNICKNAME(server, client, nickname) (std::string(":") + server + " 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
// 433
#define ERR_NICKNAMEINUSE(server, nickname) (std::string(":") + server + " 433 " + nickname + " " + nickname + " :Nickname is already in use\r\n")
// 436
#define ERR_NICKCOLLISION(server, nickname) (std::string(":") + server + " 436 " + nickname + " :Nickname collision KILL \r\n")


// 441
# define ERR_USERNOTINCHANNEL(server, clientNick, userNick, channel) (std::string(":"  + server + " 441 " + clientNick + " " + userNick + " " + channel + " :They aren't on that channel\r\n"))
// 442
#define ERR_NOTONCHANNEL(server, client, channel) (std::string(":") + server + " 442 " + client + " " + channel + " :You're not on that channel\r\n") 


// 443 
#define ERR_USERONCHANNEL(server, client, nickname, channel) (std::string(":") + server + " 443 " + client + " " + nickname + " " + channel + " :is already on channel\r\n") 
// 461
#define ERR_NEEDMOREPARAMS(server, commandName) (std::string(":") + server + " 461 " + commandName + " :Not enough parameters\r\n")
// 462
#define ERR_ALREADYREGISTERED(server, nickname) (std::string(":") + server + " 462 " + nickname + " :You may not reregister.\r\n")
// 464
#define ERR_PASSWDMISMATCH(server, nickname) (std::string(":") + server + " 464 " + nickname + " :Password incorrect\r\n")
// 482
#define ERR_CHANOPRIVSNEEDED(server, client, channel) (std::string(":") + server + " 482 " + client + " " + channel + " :You're not channel operator\r\n")

// Custom Messages
#define NICK_SUCCESS(nickname1, nickname2) (std::string(":") + nickname1 + " NICK " + nickname2 + "\r\n")
#define USER_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :User created successfully!\r\n")
#define PASS_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :Password set successfully !\r\n")
#define PONG_SUCCESS(server, token) (std::string(":") + server + " PONG :" + token + "\r\n")

#define KICK_MSG(kicker, channel, nick, reason) (std::string(":") + kicker + " KICK " + channel + " " + nick + " " + reason + "\r\n")
// KICK <channel> <nick> [<reason>]
#define KILL(server, nickname, reason ) (std::string(":") + server + " KILL " + nickname + " :" + server + "\r\n")
 
// JOIN
#define JOIN_MSG(server, channel, nickname) (std::string(":") + nickname + "!" + server + " JOIN " + channel + " " +  nickname + "\r\n")
// #define RPL_TOPIC(server, nickname, channel, topic) (std::string(":") + server + " 332 " + nickname + " " + channel + " :" + topic + "\r\n")
#define ERR_BADCHANMASK(channel) (std::string(":") + channel + " 476 " + ":Bad Channel Mask\r\n")
#define ERR_BADCHANNELKEY(server, nickname, channel) (std::string(":") + server +  " 475 " + nickname + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_TOOMANYCHANNELS(server, nickname, channel) (std::string(":") + server +  " 405 " + nickname + " " + channel + " :You have joined too many channels\r\n") 


// RPL_NAMREPLY (353) 
//  "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
// :server_name 353 your_nickname = #channel :user1 user2 user3 ...

#define PRIVMSG(server, channel, message) (std::string(":") + server + " PRIVMSG " + channel + " :" + message)

#endif



// "<client> <nick> :Nickname is already in use"