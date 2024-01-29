#pragma once

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>
#include <cstdlib>

#include <vector>
#include <map>
#include <set>

class Client;
class Server;
class Channel;


#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"


std::vector<std::string>	split(const std::string &input, const std::string &separator = " ");
typedef std::vector<std::string>::iterator StringVectorIterator;
std::string					join(StringVectorIterator itBegin, StringVectorIterator itEnd, const std::string& separator);

#define MESSAGE_DELIMITER "\r\n"

/* Error Responses */
#define ERR_NOTEXTTOSEND(source)                        ":" + source + " 412 " + source + " :No text to send" + MESSAGE_DELIMITER
#define ERR_NORECIPIENT(source, command)                ":" + source + " 411 " + source + " " + command + " :No recipient given (" + command + ")" + MESSAGE_DELIMITER
#define ERR_ERRONEUSNICKNAME(source)                    ":" + source + " 432 " + source + " :Erroneous nickname" + MESSAGE_DELIMITER
#define ERR_NOTREGISTERED(source)                       ":" + source + " 451 " + source + " :You have not registered" + MESSAGE_DELIMITER
#define ERR_ALREADYREGISTERED(source)                   ":" + source + " 462 " + source + " :You may not register" + MESSAGE_DELIMITER
#define ERR_PASSWDMISMATCH(source)                      ":" + source + " 464 " + source + " :Password is incorrect" + MESSAGE_DELIMITER
#define ERR_NONICKNAMEGIVEN(source)                     ":" + source + " 431 " + source + " :Nickname not given" + MESSAGE_DELIMITER
#define ERR_NICKNAMEINUSE(source)                       ":" + source + " 433 " + source + " " + source  + " :Nickname is already in use" + MESSAGE_DELIMITER
#define ERR_UNKNOWNCOMMAND(source, command)             ":" + source + " 421 " + source + " " + command + " :Unknown command" + MESSAGE_DELIMITER
#define ERR_NEEDMOREPARAMS(source, command)             ":" + source + " 461 " + source + " " + command + " :Not enough parameters" + MESSAGE_DELIMITER
#define ERR_USERONCHANNEL(source, channel)              ":" + source + " 443 " + source + " " + channel + " :is already on channel" + MESSAGE_DELIMITER
#define ERR_NOSUCHSERVER(source, server)                ":" + source + " 402 " + source + " " + server + " :No such server" + MESSAGE_DELIMITER
#define ERR_TOOMANYCHANNELS(source, channel)            ":" + source + " 405 " + source + " " + channel + " :You have joined too many channels" + MESSAGE_DELIMITER
#define ERR_NOTONCHANNEL(source, channel)               ":" + source + " 442 " + source + " " + channel + " :You're not on that channel" + MESSAGE_DELIMITER
#define ERR_NOSUCHCHANNEL(source, channel)              ":" + source + " 403 " + source + " " + channel + " :No such channel" + MESSAGE_DELIMITER
#define ERR_BADCHANNELKEY(source, channel)              ":" + source + " 475 " + source + " " + channel + " :Cannot join channel (+k)" + MESSAGE_DELIMITER
#define ERR_CHANNELISFULL(source, channel)              ":" + source + " 471 " + source + " " + channel + " :Cannot join channel (+l)" + MESSAGE_DELIMITER
#define ERR_CANNOTSENDTOCHAN(source, channel)           ":" + source + " 404 " + source + " " + channel + " :Cannot send to channel" + MESSAGE_DELIMITER
#define ERR_CHANOPRIVSNEEDED(source, channel)           ":" + source + " 482 " + source + " " + channel + " :You're not channel operator" + MESSAGE_DELIMITER
#define ERR_NOSUCHNICK(source, nickname)                ":" + source + " 401 " + source + " " + nickname + " :No such nick/channel" + MESSAGE_DELIMITER
#define ERR_USERNOTINCHANNEL(source, nickname, channel) ":" + source + " 441 " + source + " " + nickname + " " + channel + " :They aren't on that channel" + MESSAGE_DELIMITER
#define ERR_UNKNOWNCOMMAND(source, command)             ":" + source + " 421 " + source + " " + command + " :Unknown command" + MESSAGE_DELIMITER

/* Numeric Responses */
#define RPL_WELCOME(source)                             ":" + source + " 001 " + source + " :Welcome " + source + " to the ft_irc network" + MESSAGE_DELIMITER
#define RPL_TOPIC(source, channel, topic)               ":" + source + " 332 " + source + " " + channel + " :" + topic + MESSAGE_DELIMITER
#define RPL_NOTOPIC(source, channel)                    ":" + source + " 331 " + source + " " + channel + " :No topic is set" + MESSAGE_DELIMITER
#define RPL_PRIVMSG(source, target, message)            ":" + source + " PRIVMSG " + target + " :" + message + MESSAGE_DELIMITER
#define RPL_KICK(source, channel, target, reason)       ":" + source + " KICK " + channel + " " + target + " :" + reason + MESSAGE_DELIMITER
#define RPL_JOIN(source, channel)                       ":" + source + " JOIN " + channel + MESSAGE_DELIMITER