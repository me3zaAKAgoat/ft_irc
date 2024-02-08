# COMMANDS

- [x] PASS
- [x] NICK
- [x] USER
- [x] QUIT
- [x] JOIN
- [x] PART
- [x] KICK - OPER
- [x] MODE itkol - OPER
- [x] TOPIC - OPER
- [x] INVITE - OPER
- [X] PRIVMSG
- [x] NOTICE

# NOTES / TASKS

- check sys calls
- visit networking functions and understand them as best as possible
- how do u handle suspended clients CTRL Z 
- remove time functions? are they allowed?
- should server socket be non blocking?

# Non blocking io

A non blocking socket will not hang the flow of execution if a recv/write sys call is used on it, it will return -1 and raise EWOULDBLOCK. in non threaded programming this is crucial so the server is responsive to all users at all times

# REFERENCES

https://datatracker.ietf.org/doc/html/rfc1459#page-14
https://www.irchelp.org/protocol/rfc/chapter6.html


