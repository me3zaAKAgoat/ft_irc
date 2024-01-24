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

commands we should implement :
PASS
NICK
USER
QUIT
JOIN
PART
KICK
MODE
PRIVMSG
NOTICE

# NOTES

- this wether this current form of code is broken or not and fix all isues with networking before making all the commands
- do we need any structures for parsed commands or should extraction of arguments be done through special functions that act on the string of the command itself
- should irc.hpp exist?
- commands need a huge rehaul, a common parent class for all commands is probably needed
- parsing needs to be studied more
- visit all network functions and code and check if theres anything that is missing (probably better to ignore until issues are met)
- use of default arguments (ex: Command(Server* srv, bool auth = true);)
- rehaul client class
