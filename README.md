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