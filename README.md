# COMMANDS

NORMAL CLIENT:
- PASS
- NICK
- USER usr O * :realname
- JOIN
- PRIVMSG
- QUIT

OP:
- KICK
- INVITE
- TOPIC
- MODE

commands we should implement :
PASS
NICK
USER
QUIT
JOIN
PART
KICK
MODE
TOPIC
INVITE
PRIVMSG
NOTICE
PING
WHOIS

# NOTES / TASKS

- standarize the use of perror/throw runtime_error/std::cerr
- use response message macros
- visit all network functions and code and check if theres anything that is missing (probably better to ignore until issues are met)
- !!!!!!! CHECK ALL SYS CALL RETURNS ALL OF THEM !!!!!
- finish join command
- learn more about non blocking I/O
- read more about server socket setup functions
- ask peers for all the commands that should be implemented (dont be a faggot and ask for the reference)
- theres a buffering problem somewhere (setup client server connection and try to register)	
- questions about cr lf (will we use both chars are delimiters)
- will we use command prefixes? (most likely not)
- command validity specification
```
Raw messages will only contain printable ASCII characters (ordinals 32-126).
```
- link the reply message reference in this document !!
- MODE -> PRIVMSG -> NOTICE
- pass client to command functions by reference or by pointer?

https://datatracker.ietf.org/doc/html/rfc1459#page-14	