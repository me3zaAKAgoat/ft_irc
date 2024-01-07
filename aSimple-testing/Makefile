CPP = c++
CPP_FLAGS = -Wall -Werror -Wextra -std=c++98

SERVERNAME = server
CLIENTNAME = client

SERVSRC = server.cpp
CLIENTSRC = client.cpp
SERVOBJ = $(SERVSRC:.cpp=.o)
CLIENTOBJ = $(CLIENTSRC:.cpp=.o)
HEADERS = 

all: $(SERVERNAME) $(CLIENTNAME)

$(SERVERNAME): $(SERVOBJ)
	$(CPP) $(CPP_FLAGS) $(SERVOBJ) -o $@
$(CLIENTNAME): $(CLIENTOBJ)
	$(CPP) $(CPP_FLAGS) $(CLIENTOBJ) -o $@

%.o : %.cpp $(HEADERS)
	$(CPP) $(CPP_FLAGS) -c $< -o $@

clean:
	rm -rf $(SERVOBJ)
	rm -rf $(CLIENTOBJ)

fclean: clean
	rm -rf $(SERVERNAME)
	rm -rf $(CLIENTNAME)

re: fclean all