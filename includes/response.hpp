#pragma once

#define MESSAGE_DELIMITER "\r\n"

/* Error Responses */
#define ERR_NOTEXTTOSEND(source)                        ":irc.server.com 412 " + source + " :No text to send" + MESSAGE_DELIMITER
#define ERR_NORECIPIENT(source, command)                ":irc.server.com 411 " + source + " " + command + " :No recipient given (" + command + ")" + MESSAGE_DELIMITER
#define ERR_ERRONEUSNICKNAME(source)                    ":irc.server.com 432 " + source + " :Erroneous nickname" + MESSAGE_DELIMITER
#define ERR_NOTREGISTERED(source)                       ":irc.server.com 451 " + source + " :You have not registered" + MESSAGE_DELIMITER
#define ERR_ALREADYREGISTERED(source)                   ":irc.server.com 462 " + source + " :You may not register" + MESSAGE_DELIMITER
#define ERR_PASSWDMISMATCH(source)                      ":irc.server.com 464 " + source + " :Password is incorrect" + MESSAGE_DELIMITER
#define ERR_NONICKNAMEGIVEN(source)                     ":irc.server.com 431 " + source + " :Nickname not given" + MESSAGE_DELIMITER
#define ERR_NICKNAMEINUSE(source)                       ":irc.server.com 433 " + source + " " + source  + " :Nickname is already in use" + MESSAGE_DELIMITER
#define ERR_UNKNOWNCOMMAND(source, command)             ":irc.server.com 421 " + source + " " + command + " :Unknown command" + MESSAGE_DELIMITER
#define ERR_NEEDMOREPARAMS(source, command)             ":irc.server.com 461 " + source + " " + command + " :Not enough parameters" + MESSAGE_DELIMITER
#define ERR_USERONCHANNEL(source, channel)              ":irc.server.com 443 " + source + " " + channel + " :is already on channel" + MESSAGE_DELIMITER
#define ERR_NOSUCHSERVER(source, server)                ":irc.server.com 402 " + source + " " + server + " :No such server" + MESSAGE_DELIMITER
#define ERR_TOOMANYCHANNELS(source, channel)            ":irc.server.com 405 " + source + " " + channel + " :You have joined too many channels" + MESSAGE_DELIMITER
#define ERR_NOTONCHANNEL(source, channel)               ":irc.server.com 442 " + source + " " + channel + " :You're not on that channel" + MESSAGE_DELIMITER
#define ERR_NOSUCHCHANNEL(source, channel)              ":irc.server.com 403 " + source + " " + channel + " :No such channel" + MESSAGE_DELIMITER
#define ERR_BADCHANNELKEY(source, channel)              ":irc.server.com 475 " + source + " " + channel + " :Cannot join channel (+k)" + MESSAGE_DELIMITER
#define ERR_CHANNELISFULL(source, channel)              ":irc.server.com 471 " + source + " " + channel + " :Cannot join channel (+l)" + MESSAGE_DELIMITER
#define ERR_CANNOTSENDTOCHAN(source, channel)           ":irc.server.com 404 " + source + " " + channel + " :Cannot send to channel" + MESSAGE_DELIMITER
#define ERR_CHANOPRIVSNEEDED(source, channel)           ":irc.server.com 482 " + source + " " + channel + " :You're not channel operator" + MESSAGE_DELIMITER
#define ERR_NOSUCHNICK(source, nickname)                ":irc.server.com 401 " + source + " " + nickname + " :No such nick/channel" + MESSAGE_DELIMITER
#define ERR_USERNOTINCHANNEL(source, nickname, channel) ":irc.server.com 441 " + source + " " + nickname + " " + channel + " :They aren't on that channel" + MESSAGE_DELIMITER
#define ERR_UNKNOWNCOMMAND(source, command)             ":irc.server.com 421 " + source + " " + command + " :Unknown command" + MESSAGE_DELIMITER
#define ERR_UNKNOWNMODE(source, mode)                   ":irc.server.com 472 " + source + " " + mode + " :is unknown mode char to me" + MESSAGE_DELIMITER
#define ERR_INVALIDMODEPARAM(source, mode, param)       ":irc.server.com 696 " + source + " " + mode + " " + param + " :Invalid MODE flag paramater" + MESSAGE_DELIMITER
#define ERR_INVITEONLYCHAN(channel)                     ":irc.server.com 473 " + channel + " :Cannot join channel (+i)" + MESSAGE_DELIMITER

/* Numeric Responses */
#define RPL_WELCOME(nickname)                           ":irc.server.com 001 :Welcome " + nickname + " to the ft_irc network" + MESSAGE_DELIMITER
#define RPL_TOPIC(channel, topic)               		":irc.server.com 332 " + channel + " :" + topic + MESSAGE_DELIMITER
#define RPL_NOTOPIC(channel)                            ":irc.server.com 331 " + channel + " :No topic is set" + MESSAGE_DELIMITER
#define RPL_INVITING(source, nickname, channel)         ":irc.server.com 341 " + source + " " + nickname + " " + channel + " :Inviting " + nickname + " to channel " + channel + MESSAGE_DELIMITER
#define RPL_NAMREPLY(channel, nicknames)                ":irc.server.com 353 :" + nicknames + MESSAGE_DELIMITER
#define RPL_PRIVMSG(source, target, message)            ":irc.server.com PRIVMSG " + target + " :" + message + MESSAGE_DELIMITER
#define RPL_KICK(source, channel, target, reason)       ":irc.server.com KICK " + channel + " " + target + " :" + reason + MESSAGE_DELIMITER
#define RPL_JOIN(source, channel)                       ":irc.server.com JOIN " + channel + MESSAGE_DELIMITER
#define RPL_NOTICE(source, target, message)             ":irc.server.com NOTICE " + target + " :" + message + MESSAGE_DELIMITER