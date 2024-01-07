CPP = c++
CPP_FLAGS = -Wall -Werror -Wextra -std=c++98

NAME = ircserv

SRC = src/main.cpp src/Server.cpp src/Client.cpp \
	  src/Commands/NICK.cpp src/Commands/PASS.cpp src/Commands/USER.cpp
OBJ = $(SRC:.cpp=.o)
HEADERS = includes/Server.hpp includes/Client.hpp includes/Irc.hpp includes/Commands.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(CPP_FLAGS) $(OBJ) -o $@

%.o : %.cpp $(HEADERS)
	$(CPP) -I./includes $(CPP_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all