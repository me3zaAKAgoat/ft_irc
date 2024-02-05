#pragma once

#define MESSAGE_DELIMITER "\r\n"

/* Error Responses */
#define ERR_NOTEXTTOSEND(source)                        "412 " + source + " :No text to send" + MESSAGE_DELIMITER
#define ERR_NORECIPIENT(source, command)                "411 " + source + " " + command + " :No recipient given (" + command + ")" + MESSAGE_DELIMITER
#define ERR_ERRONEUSNICKNAME(source)                    "432 " + source + " :Erroneous nickname" + MESSAGE_DELIMITER
#define ERR_NOTREGISTERED(source)                       "451 " + source + " :You have not registered" + MESSAGE_DELIMITER
#define ERR_ALREADYREGISTERED(source)                   "462 " + source + " :You may not register" + MESSAGE_DELIMITER
#define ERR_PASSWDMISMATCH(source)                      "464 " + source + " :Password is incorrect" + MESSAGE_DELIMITER
#define ERR_NONICKNAMEGIVEN()                           std::string("431 :Nickname not given") + MESSAGE_DELIMITER
#define ERR_NICKNAMEINUSE(source)                       "433 " + source + " " + source  + " :Nickname is already in use" + MESSAGE_DELIMITER
#define ERR_UNKNOWNCOMMAND(command)                     "421 " + command + " :Unknown command" + MESSAGE_DELIMITER
#define ERR_NEEDMOREPARAMS(source, command)             "461 " + source + " " + command + " :Not enough parameters" + MESSAGE_DELIMITER
#define ERR_USERONCHANNEL(source, channel)              "443 " + source + " " + channel + " :is already on channel" + MESSAGE_DELIMITER
#define ERR_NOSUCHSERVER(source, server)                "402 " + source + " " + server + " :No such server" + MESSAGE_DELIMITER
#define ERR_TOOMANYCHANNELS(source, channel)            "405 " + source + " " + channel + " :You have joined too many channels" + MESSAGE_DELIMITER
#define ERR_NOTONCHANNEL(source, channel)               "442 " + source + " " + channel + " :You're not on that channel" + MESSAGE_DELIMITER
#define ERR_NOSUCHCHANNEL(source, channel)              "403 " + source + " " + channel + " :No such channel" + MESSAGE_DELIMITER
#define ERR_BADCHANNELKEY(source, channel)              "475 " + source + " " + channel + " :Cannot join channel (+k)" + MESSAGE_DELIMITER
#define ERR_CHANNELISFULL(source, channel)              "471 " + source + " " + channel + " :Cannot join channel (+l)" + MESSAGE_DELIMITER
#define ERR_CANNOTSENDTOCHAN(source, channel)           "404 " + source + " " + channel + " :Cannot send to channel" + MESSAGE_DELIMITER
#define ERR_CHANOPRIVSNEEDED(source, channel)           "482 " + source + " " + channel + " :You're not channel operator" + MESSAGE_DELIMITER
#define ERR_NOSUCHNICK(source, nickname)                "401 " + source + " " + nickname + " :No such nick/channel" + MESSAGE_DELIMITER
#define ERR_USERNOTINCHANNEL(source, nickname, channel) "441 " + source + " " + nickname + " " + channel + " :They aren't on that channel" + MESSAGE_DELIMITER
#define ERR_UNKNOWNMODE(source, mode)                   "472 " + source + " " + mode + " :is unknown mode char to me" + MESSAGE_DELIMITER
#define ERR_INVALIDMODEPARAM(source, mode, param)       "696 " + source + " " + mode + " " + param + " :Invalid MODE flag paramater" + MESSAGE_DELIMITER
#define ERR_INVITEONLYCHAN(channel)                     "473 " + channel + " :Cannot join channel (+i)" + MESSAGE_DELIMITER

/* Numeric Responses */
#define RPL_WELCOME(nickname)                           "001 " + nickname + " :Welcome " + nickname + " to the ft_irc network" + MESSAGE_DELIMITER
#define RPL_TOPIC(joiner, channel, topic)               "332 " + joiner + " " + channel + " :" + topic + MESSAGE_DELIMITER
#define RPL_NOTOPIC(joiner, channel)                    "331 " + joiner + " " + channel + " :No topic is set." + MESSAGE_DELIMITER
#define RPL_NAMREPLY(joiner, channel, nicknames)        "353 " + joiner + " = " + channel + " :" + nicknames + MESSAGE_DELIMITER
#define RPL_INVITING(source, nickname, channel)         "341 " + source + " " + nickname + " " + channel + " :Inviting " + nickname + " to channel " + channel + MESSAGE_DELIMITER
#define RPL_PRIVMSG(source, target, message)            ":" + source + " PRIVMSG " + target + " :" + message + MESSAGE_DELIMITER
#define RPL_NOTICE(source, target, message)             ":" + source + " NOTICE " + target + " :" + message + MESSAGE_DELIMITER
#define RPL_KICK(source, channel, target, reason)       "KICK " + channel + " " + target + " :" + reason + MESSAGE_DELIMITER
#define RPL_JOIN(source, channel)                       "JOIN " + channel + MESSAGE_DELIMITER
#define RPL_NICKCHANGE(oldNick, newNick)                ":" + oldNick + " NICK " + newNick + MESSAGE_DELIMITER