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
- create a channel class
- theres a buffering problem somewhere
- questions about cr lf (will we use both chars are delimiters)
- will we use command prefixes? (most likely not)
- all commands should take the command struct as first parameter then whatever additional needed arguments afterwards
- command validity specification
```
Raw messages will only contain printable ASCII characters (ordinals 32-126).
Raw messages will start with a command, which will not contain any spaces.
Any number of arguments may follow a command. Arguments are delimited by one or more spaces followed by a colon (<space>: and <space><space>: are both valid delimiters).
	Any arguments that do not contain spaces may omit the colon from the delimiter so long as all previous arguments (if any) have omitted the colon.
Colons will not appear within arguments or commands.
Arguments will not begin with a space.
```
- link the reply message reference in this document !!


https://datatracker.ietf.org/doc/html/rfc1459#page-14	