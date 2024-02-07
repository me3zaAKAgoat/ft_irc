CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g
NAME = ircserv
BONUS_NAME = BMOBot

HEADERS = includes/Server.hpp includes/Client.hpp includes/Irc.hpp includes/Commands.hpp includes/response.hpp
SRCS = src/main.cpp src/Server.cpp src/Client.cpp src/Channel.cpp src/util.cpp src/networking.cpp\
	src/commands/NICK.cpp src/commands/PASS.cpp src/commands/USER.cpp src/commands/JOIN.cpp\
	src/commands/PART.cpp src/commands/PRIVMSG.cpp src/commands/QUIT.cpp src/commands/INVITE.cpp\
	src/commands/TOPIC.cpp src/commands/KICK.cpp src/commands/MODE.cpp src/commands/NOTICE.cpp\
	src/commands/parser.cpp

BONUS_HEADERS = includes/BMOBot.hpp includes/Irc.hpp
BONUS_SRCS = src/bot/BMOBot.cpp src/bot/main.cpp src/util.cpp src/commands/parser.cpp

OBJECTS = $(SRCS:.cpp=.o)
BONUS_OBJECTS = $(BONUS_SRCS:.cpp=.o)

all: $(NAME)

bonus : $(BONUS_NAME)

$(NAME) : $(OBJECTS)
	$(CPP) $(CPPFLAGS) -o $@ $(OBJECTS)

$(BONUS_NAME) : $(BONUS_OBJECTS)
	$(CPP) $(CPPFLAGS) -o $@ $(BONUS_OBJECTS)

%.o : %.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -I./includes -c $< -o $@

src/bot/%.o : src/bot/%.cpp $(BONUS_HEADERS)
	$(CPP) $(CPPFLAGS) -I./includes -c $< -o $@

clean :
	rm -rf $(OBJECTS) $(BONUS_OBJECTS)

fclean :
	rm -rf $(OBJECTS) $(BONUS_OBJECTS)
	rm -rf $(NAME) $(BONUS_NAME)

re : fclean all

.PHONY : fclean re clean all