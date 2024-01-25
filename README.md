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
- parsing needs to be studied more
- visit all network functions and code and check if theres anything that is missing (probably better to ignore until issues are met)
- wtf are the join functions arguments
- !!!!!!! CHECK ALL SYS CALL RETURNS ALL OF THEM !!!!!
- finish join command
- learn more about non blocking I/O
- read more about server socket setup functions
- ask peers for all the commands that should be implemented (dont be a faggot and ask for the reference)
- create a channel class
- theres a buffering problem somewhere