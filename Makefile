CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
NAME = ircserv 

HEADERS = includes/Server.hpp includes/Client.hpp includes/Irc.hpp includes/Commands.hpp
SRCS = src/main.cpp src/Server.cpp src/Client.cpp src/Channel.cpp src/util.cpp\
	src/commands/NICK.cpp src/commands/PASS.cpp src/commands/USER.cpp src/commands/JOIN.cpp\
	src/commands/PART.cpp src/commands/PRIVMSG.cpp src/commands/QUIT.cpp src/commands/INVITE.cpp\
	src/commands/TOPIC.cpp src/commands/KICK.cpp\
	src/commands/parser.cpp

OBJECTS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJECTS)
	$(CPP) $(CPPFLAGS) -o $@ $(OBJECTS)

%.o : %.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -I./includes -c $< -o $@

clean :
	rm -rf $(OBJECTS)

fclean :
	rm -rf $(OBJECTS)
	rm -rf $(NAME)

re : fclean all

.PHONY : fclean re clean all