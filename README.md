# COMMANDS

- part should send a reply to client so the channel gets automatically removed
- invite should send a message to invited user
- kick + part + join commands broadcasting of replies may not be correct
- ctrl c should do cleanup from channels and stuff
- a lot of channels to join edge cases
- set limit for how many characters could be read without crlf before throwing away the message 512
- server socket should be closed prior to full construciton
- bot nickname
- server oprators

- there wil be a problem where if a message is longer than 512 characters, when the next read for that client will come the first part of the message will be the truncated prior message + the newer one (im not sure)
- 005 reply to send the client information about server config
- tell ekenane to understand dcc a little more hhhh
- file transfer does not work !!!!! need hostnames of clients to be registered and privmsg to correctly indicate hosts
- badkey error reply in join if key argument is unexistent instead of needmoreparams
- can the 500 limit cause any issues in messages that are beyond 500 in length
- [x] return on poll -1
- [x] bot dosent work (001 if condition)
- [x] change bot read to 500 limit also
