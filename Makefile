CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
NAME = ircserv 

HEADERS = includes/Server.hpp includes/Client.hpp includes/Irc.hpp includes/Commands.hpp
SRCS = src/main.cpp src/Server.cpp src/Client.cpp \
	src/commands/NICK.cpp src/commands/PASS.cpp src/commands/USER.cpp
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