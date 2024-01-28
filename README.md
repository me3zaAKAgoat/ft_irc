# COMMANDS

- [x] PASS
- [x] NICK
- [x] USER
- [x] QUIT
- [x] JOIN
- [x] PART
- [] KICK - OPER
- [] MODE itkol - OPER
- [x] TOPIC - OPER
- [x] INVITE - OPER
- [X] PRIVMSG
- [] NOTICE

# NOTES / TASKS

- standarize the use of perror/throw runtime_error/std::cerr
- use response message macros
- visit all network functions and code and check if theres anything that is missing (probably better to ignore until issues are met)
- !!!!!!! CHECK ALL SYS CALL RETURNS ALL OF THEM !!!!!
- learn more about non blocking I/O
- read more about server socket setup functions
- ask peers for all the commands that should be implemented (dont be a faggot and ask for the reference)
- theres a buffering problem somewhere (setup client server connection and try to register)	
- questions about cr lf (will we use both chars are delimiters)
- will we use command prefixes? (most likely not)
- link the reply message reference in this document !!
- pass client to command functions by reference or by pointer?
- quit command causes a certain send operation to fail!

https://datatracker.ietf.org/doc/html/rfc1459#page-14
https://www.irchelp.org/protocol/rfc/chapter6.html


