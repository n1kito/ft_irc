#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

// 001
#define RPL_WELCOME(server, nickname) (std::string(":") + server + " 001 " + nickname + " :Welcome to the " + server + " Network, " + nickname + "\r\n")
// 002
#define RPL_YOURHOST(server, nickname, serverName, version) (std::string(":") + server + " 002 " + nickname + " :Your host is " + serverName + ", running version " + version + "\r\n")
// 003
#define RPL_CREATED(server, nickname, datetime) (std::string(":") + server + " 003 " + nickname + " :This server was created " + datetime + "\r\n")
// 004
#define RPL_MYINFO(server, nickname, serverName, version, userModes, channelModes) (std::string(":") + server + " 004 " + nickname + " :" + serverName + " " + version + " " + userModes + " " + channelModes + "\r\n")

// TOPIC
// Custom
#define TOPIC_SUCCESS(server, nickname, channel, topic) (std::string(":") + nickname + "!" + server + " TOPIC " + channel + " :" + topic + "\r\n")
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
// 404
#define ERR_CANNOTSENDTOCHAN(server, nickname, channel) (std::string(":") + server + " 404 " + nickname + " " + channel + " :Cannot send to channel\r\n")
// 411
#define ERR_NORECIPIENT(server, nickname) (std::string(":") + server + " 411 " + nickname + " :No recipient given\r\n")
// 412
#define ERR_NOTEXTTOSEND(server, nickname) (std::string(":") + server, + " 412 " + nickname + " :No text to send\r\n")
// 431
#define ERR_NONICKNAMEGIVEN(server) (std::string(":") + server + " 431 " + " :No nickname given\r\n")
// 432
#define ERR_ERRONEUSNICKNAME(server, client, nickname) (std::string(":") + server + " 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
// 433
#define ERR_NICKNAMEINUSE(server, nickname) (std::string(":") + server + " 433 " + nickname + " " + nickname + " :Nickname is already in use\r\n")
// 436
#define ERR_NICKCOLLISION(server, nickname) (std::string(":") + server + " 436 " + nickname + " :Nickname collision KILL \r\n")
// 441
#define ERR_USERNOTINCHANNEL(server, client, nickname, channel) (std::string(":") + server + " 441 " + client + " " + nickname + " " + channel + " :They aren't on that channel\r\n")
// 442
#define ERR_NOTONCHANNEL(server, client, channel) (std::string(":") + server + " 442 " + client + " " + channel + " :You're not on that channel\r\n") 
// 443
#define ERR_USERONCHANNEL(server, nickname, invitedUserNickname, channel) (std::string(":") + server + " 443 " + nickname + " " + channel + " " + invitedUserNickname + " :is already on channel\r\n") 
// 461
#define ERR_NEEDMOREPARAMS(server, nickname, commandName) (std::string(":") + server + " 461 " + nickname + " " + commandName + " :Not enough parameters\r\n")
// 462
#define ERR_ALREADYREGISTERED(server, nickname) (std::string(":") + server + " 462 " + nickname + " :You may not reregister.\r\n")
// 464
#define ERR_PASSWDMISMATCH(server, nickname) (std::string(":") + server + " 464 " + nickname + " :Password incorrect\r\n")
// 482
#define ERR_CHANOPRIVSNEEDED(server, client, channel) (std::string(":") + server + " 482 " + client + " " + channel + " :You're not channel operator\r\n")

// MODE
// 502
#define ERR_USERSDONTMATCH(server, client) (std::string(":") + server + " 502 " + client + " :Cannot change mode for other users\r\n")
// 221
#define RPL_UMODEIS(server, client, modes) (std::string(":") + server + " 221 " + client + " " + modes + "\r\n")
// 501
#define ERR_UMODEUNKNOWNFLAG(server, client, modeCharacter) (std::string(":") + server + " 501 " + client + " :Unknown mode flag " + modeCharacter + "\r\n") 
// #define MODE_MSG(server, client, target, modes) (std::string(":") + server + "!" + client + " MODE " + target + " " + modes + "\r\n")
#define MODE_MSG(server, clientNickname, clientUsername, target, modes) (std::string(":") + clientNickname + "!" + clientUsername + "@" + server + " MODE " + target + " " + modes + "\r\n")
// 324
#define RPL_CHANNELMODEIS(server, client, channel, modes, modeArguments) (std::string(":") + server + " 324 " + client + " " + channel + " " + modes + modeArguments + "\r\n")
// 329
#define RPL_CREATIONTIME(server, client, channel, creationTime) (std::string(":") + server + " 329 " + client + " " + channel + " " + creationTime + "\r\n")
// 472
#define ERR_UNKNOWNMODE(server, client, modeChar) (std::string(":") + server + " 472 " + client + " " + modeChar + " :is unknown mode char to me\r\n") 
// Custom
#define CSTM_MAXCLIENTS(server, client) (std::string(":") + server + " 999 " + client + " :+l: channels can welcome up to 100 users\r\n")

// TODO: check custom message codes and whether we need them at all or not
// Custom Messages
#define NICK_SUCCESS(nickname1, nickname2) (std::string(":") + nickname1 + " NICK " + nickname2 + "\r\n")
#define USER_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :User created successfully!\r\n")
#define PASS_SUCCESS(server, nickname) (std::string(":") + server + " 001 " + nickname + " :Password set successfully !\r\n")
#define PONG_SUCCESS(server, token) (std::string(":") + server + " PONG :" + token + "\r\n")
// #define INVITE(server, sourceNickname, channel) (std::string(":") + server + " " + sourceNickname + " invites you to " + channel + "\r\n")
// TODO: check that server is not needed here
#define INVITE(sourceNickname, targetNickname, channel) (std::string(":") + sourceNickname + " INVITE " + targetNickname + " " + channel + "\r\n")
// :<inviter> INVITE <target> <channel>

// :chacha!~coucou@C4118BBD.B270E442.5F584402.IP KICK #channel1 jee_ :chacha
// :nickname!~username@hostname KICK channel kickedNickname :reason
#define KICK_MSG(kickerNick, kickerUsername, channel, kickedNick, reason) (std::string(":") + kickerNick + "!~" + kickerUsername + "@localhost" + " KICK " + channel + " " + kickedNick + " :" + reason + "\r\n")
// #define KICK_MSG(server, kicker, channel, nick, reason) (std::string(":") + kicker + " KICK " + channel + " " + nick + " " + reason + "\r\n")
#define KILL(server, nickname, reason ) (std::string(":") + server + " KILL " + nickname + " :" + server + "\r\n")

// JOIN
#define JOIN_MSG(server, channel, nickname, username) (std::string(":") + nickname + "!" + username + "@" + server + " JOIN :" + channel + "\r\n")
#define PART_MSG(server, channel, nickname, message) (std::string(":") + channel + "!" + server + " PART " + nickname +" :" + message+ "\r\n")

// 448
#define ERR_BADCHANAME(server, nickname, channel) (std::string(":") + server + " 448 " + nickname + " :Cannot join channel " + channel + ": Channel name must start with a hash mark (#)\r\n")
// 476 
#define ERR_BADCHANMASK(server, channel) (std::string(":") + server + " 476 " + channel + " :Bad Channel Mask\r\n")
// 475
#define ERR_BADCHANNELKEY(server, nickname, channel) (std::string(":") + server +  " 475 " + nickname + " " + channel + " :Cannot join channel (+k)\r\n")
// 405
#define ERR_TOOMANYCHANNELS(server, nickname, channel) (std::string(":") + server +  " 405 " + nickname + " " + channel + " :You have joined too many channels\r\n") 
// 353
#define ERR_CHANNELISFULL(server, nickname, channel) (std::string(":") + server + " 471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n")
// 473
#define ERR_INVITEONLYCHAN(server, nickname, channel) (std::string(":") + server + " 473 " + nickname + " " + channel + " :Cannot join channel (+i)\r\n")

#define QUIT_MSG(server, nickname, username, channel, message) (std::string(":") + nickname + "!" + username + "@" + server + " QUIT " + message + "\r\n")

#define PRIVMSG(senderNickname, senderUsername, receiver, message) (std::string(":") + senderNickname + "!~" + senderUsername + "@localhost" + " PRIVMSG " + receiver + " :" + message + "\r\n")

#define NOTICE(senderNickname, senderUsername, receiver, message) (std::string(":") + senderNickname + "!~" + senderUsername + "@localhost" + " NOTICE " + receiver + " :" + message + "\r\n")
#endif