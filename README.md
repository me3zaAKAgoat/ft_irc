## Classes
- Server
- Client
- Channel


## COMMANDS

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

# NOTES

- how should parsing work
- do we need any structures for parsed commands or should extraction of arguments be done through special functions that act on the string of the command itself
- should irc.hpp exist?
- commands need a huge rehaul, a common parent class for all commands is probably needed
- parsing needs to be studied