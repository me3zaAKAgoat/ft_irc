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

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"

std::vector<std::string>	split(const std::string &input, const std::string &separator = " ");
typedef std::vector<std::string>::iterator StringVectorIterator;
std::string					join(StringVectorIterator itBegin, StringVectorIterator itEnd, const std::string& separator);

/* Error Responses */


#define ERR_NOTEXTTOSEND(source)                         ":" + source + " 412 " + source + " :No text to send" + COMMANDS_DELIMITER
#define ERR_NORECIPIENT(source, command)                 ":" + source + " 411 " + source + " " + command + " :No recipient given (" + command + ")" + COMMANDS_DELIMITER
#define ERR_ERRONEUSNICKNAME(source)                     ":" + source + " 432 " + source + " :Erroneous nickname" + COMMANDS_DELIMITER
#define ERR_NOTREGISTERED(source)                        ":" + source + " 451 " + source + " :You have not registered" + COMMANDS_DELIMITER
#define ERR_ALREADYREGISTERED(source)                    ":" + source + " 462 " + source + " :You may not register" + COMMANDS_DELIMITER
#define ERR_PASSWDMISMATCH(source)                       ":" + source + " 464 " + source + " :Password is incorrect" + COMMANDS_DELIMITER
#define ERR_NONICKNAMEGIVEN(source)                      ":" + source + " 431 " + source + " :Nickname not given" + COMMANDS_DELIMITER
#define ERR_NICKNAMEINUSE(source)                        ":" + source + " 433 " + source + " " + source  + " :Nickname is already in use" + COMMANDS_DELIMITER
#define ERR_UNKNOWNCOMMAND(source, command)              ":" + source + " 421 " + source + " " + command + " :Unknown command" + COMMANDS_DELIMITER
#define ERR_NEEDMOREPARAMS(source, command)              ":" + source + " 461 " + source + " " + command + " :Not enough parameters" + COMMANDS_DELIMITER
#define ERR_USERONCHANNEL(source, channel)               ":" + source + " 443 " + source + " " + channel + " :is already on channel" + COMMANDS_DELIMITER
#define ERR_NOSUCHSERVER(source, server)                 ":" + source + " 402 " + source + " " + server + " :No such server" + COMMANDS_DELIMITER
#define ERR_TOOMANYCHANNELS(source, channel)             ":" + source + " 405 " + source + " " + channel + " :You have joined too many channels" + COMMANDS_DELIMITER
#define ERR_NOTONCHANNEL(source, channel)                ":" + source + " 442 " + source + " " + channel + " :You're not on that channel" + COMMANDS_DELIMITER
#define ERR_NOSUCHCHANNEL(source, channel)               ":" + source + " 403 " + source + " " + channel + " :No such channel" + COMMANDS_DELIMITER
#define ERR_BADCHANNELKEY(source, channel)               ":" + source + " 475 " + source + " " + channel + " :Cannot join channel (+k)" + COMMANDS_DELIMITER
#define ERR_CHANNELISFULL(source, channel)               ":" + source + " 471 " + source + " " + channel + " :Cannot join channel (+l)" + COMMANDS_DELIMITER
#define ERR_CANNOTSENDTOCHAN(source, channel)            ":" + source + " 404 " + source + " " + channel + " :Cannot send to channel" + COMMANDS_DELIMITER
#define ERR_CHANOPRIVSNEEDED(source, channel)            ":" + source + " 482 " + source + " " + channel + " :You're not channel operator" + COMMANDS_DELIMITER
#define ERR_NOSUCHNICK(source, nickname)                 ":" + source + " 401 " + source + " " + nickname + " :No such nick/channel" + COMMANDS_DELIMITER
#define ERR_USERNOTINCHANNEL(source, nickname, channel)  ":" + source + " 441 " + source + " " + nickname + " " + channel + " :They aren't on that channel" + COMMANDS_DELIMITER
#define ERR_UNKNOWNMODE(source, channel, char) ":" + source + " 441 " + source + " " + channel + char + " :is unknown mode char to me" + COMMANDS_DELIMITER

/* Numeric Responses */

#define RPL_WELCOME(source)                             ":" + source + " 001 " + source + " :Welcome " + source + " to the ft_irc network" + COMMANDS_DELIMITER
#define RPL_TOPIC(source, channel, topic)               ":" + source + " 332 " + source + " " + channel + " :" + topic + COMMANDS_DELIMITER
#define RPL_NOTOPIC(source, channel)                    ":" + source + " 331 " + source + " " + channel + " :No topic is set" + COMMANDS_DELIMITER