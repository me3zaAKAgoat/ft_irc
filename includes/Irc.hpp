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
typedef typename std::vector<std::string>::iterator StringVectorIterator;
std::string					join(StringVectorIterator itBegin, StringVectorIterator itEnd, const std::string& separator);

/* Error Responses */


#define ERR_NOTEXTTOSEND(source)                        "412 " + source + " :No text to send"
#define ERR_NORECIPIENT(source, command)                "411 " + source + " " + command + " :No recipient given (" + command + ")"
#define ERR_ERRONEUSNICKNAME(nickname)                  "432 " + nickname + " :Erroneous nickname"
#define ERR_NOTREGISTERED(source)                       "451 " + source + " :You have not registered"
#define ERR_ALREADYREGISTERED(source)                   "462 " + source + " :You may not register"
#define ERR_PASSWDMISMATCH(source)                      "464 " + source + " :Password is incorrect"
#define ERR_NONICKNAMEGIVEN(source)                     "431 " + source + " :Nickname not given"
#define ERR_NICKNAMEINUSE(source)                       "433 " + source + " " + source  + " :Nickname is already in use"
#define ERR_UNKNOWNCOMMAND(source, command)             "421 " + source + " " + command + " :Unknown command"
#define ERR_NEEDMOREPARAMS(source, command)             "461 " + source + " " + command + " :Not enough parameters"
#define ERR_USERONCHANNEL(source, channel)              "443 " + source + " " + channel + " :is already on channel"
#define ERR_NOSUCHSERVER(source, server)                "402 " + source + " " + server + " :No such server"
#define ERR_TOOMANYCHANNELS(source, channel)            "405 " + source + " " + channel + " :You have joined too many channels"
#define ERR_NOTONCHANNEL(source, channel)               "442 " + source + " " + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(source, channel)              "403 " + source + " " + channel + " :No such channel"
#define ERR_BADCHANNELKEY(source, channel)              "475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANNELISFULL(source, channel)              "471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_CANNOTSENDTOCHAN(source, channel)           "404 " + source + " " + channel + " :Cannot send to channel"
#define ERR_CHANOPRIVSNEEDED(source, channel)           "482 " + source + " " + channel + " :You're not channel operator"
#define ERR_NOSUCHNICK(source, nickname)                "401 " + source + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(source, nickname, channel) "441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"

/* Numeric Responses */

#define RPL_WELCOME(source)                             "001 " + source + " :Welcome " + source + " to the ft_irc network"

