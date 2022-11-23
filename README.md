# ft_irc
This project is about creating your own IRC server. You will use an actual IRC client to connect to your server and test it. Internet is ruled by solid standards protocols that allow connected computers to interact with each other. It’s always a good thing to know.
# References
- miniircd: https://github.com/jrosdahl/miniircd
- https://www.tutorialspoint.com/c-program-to-display-hostname-and-ip-address
- https://www.geeksforgeeks.org/c-program-display-hostname-ip-address/
- https://www.tutorialspoint.com/unix_sockets/socket_server_example.html
- https://www.ibm.com/docs/ja/i/7.3?topic=ssw_ibm_i_73/apis/poll.html
- https://www.ibm.com/docs/en/i/7.2?topic=clients-example-ipv4-ipv6-client
- https://irssi.org/documentation/
- https://linuxhint.com/use-irssi-for-internet-relay-chat-ubuntu/
- https://github.com/irssi/irssi
- https://www.youtube.com/watch?v=67f9iptWqt0
- https://www.irchelp.org/clients/mac/
- https://www.makeuseof.com/tag/how-to-create-your-own-irc-chat-channel/
- https://stackoverflow.com/questions/68808597/how-can-my-irc-client-print-data-that-was-received-before-its-in-the-loop-that
- https://stackoverflow.com/questions/11079800/irc-server-configuration-possibilities
- https://ircv3.net/specs/extensions/sasl-3.1.html
- https://modern.ircdocs.horse/

# RFC Documents
- https://www.rfc-editor.org/rfc/rfc1459 - Internet Relay Chat Protocol
- https://www.rfc-editor.org/rfc/rfc2813 - Internet Relay Chat: Server Protocol
- https://www.rfc-editor.org/rfc/rfc2812 - Internet Relay Chat: Client Protocol
- https://www.rfc-editor.org/rfc/rfc2811 - Internet Relay Chat: Channel Management
- https://www.rfc-editor.org/rfc/rfc2810 - Internet Relay Chat: Architecture
## Things to do in irc:
- We need to authenticate the client user
	- This can done by the server by asking specific questions after connecting.
	- like if the client try to connect to the client with correct ip and port.
	- We connect then ask the client to provide password.
	- If the client gives a wrong password we close the connection else continue.
- Set a nick name:
- Set a user name:
- Send and recieve private messages using your reference client: one client to other client
## Channel Things:
- Create a channel:
- Allow users to join the channel that is created.
- if a message send by the user in a channel then we have to forward it to every other client.
## operator (Surprise!)
- Operators should be able to perform basic network tasks such as disconnecting and reconnecting servers as needed to prevent long-term use of bad network routing.  In recognition of this need, the protocol discussed herein provides for operators only to be able to perform such functions.  See sections 4.1.7 (SQUIT) and 4.3.5 (CONNECT).
- A more controversial power of operators is the ability  to  remove  a user  from  the connected network by 'force', i.e. operators are able to close the connection between any client and server.   The justification for  this  is delicate since its abuse is both destructive and annoying. For further details on this type of action, see section 4.6.1 (KILL).
- We must have operators:
- They have specific commands to use those are not mentioned.
## User data: (How to store user data? Vector, map, structure,)
-  A client is anything connecting to a server that is not another server.  Each client is distinguished from other clients by a unique nickname having a maximum length of nine (9) characters.
- user name;
- nick name;
- channels joined;
- type of user: operator or normal

## [IRC commands](https://www.ionos.com/digitalguide/server/know-how/irc/)
- In Internet Relay Chat, there is a set of IRC commands that you can insert in the input line and that will perform a certain action afterwards. We've rounded up a few of the most helpful ones for you:

- /help: This command is one of the most important as it lists all the commands available to you in the channel. So, for beginners, it provides a good overview of all the options on IRC.
- /rules: It is important to be aware of the applicable policies, especially as a newcomer to a channel. This command provides you with all the server rules.
- /join #channelname: As mentioned before, this command is used to enable you to join a specific channel. This allows you to jump between various channels with ease.
- /nick <new nickname>: This command enables you to change the nickname you use to communicate with others in the channel. However, try not to do this too often to avoid unnecessary confusion.
- /invite <nickname>: If you want to invite a specific person to the channel you are currently in, you can use this command. You may need to attach “#channelname” to the command if only channel members are allowed to invite others.
- /msg <nickname>: If you want to send a text message to only one person and not everyone in the chat, use this command to start a private conversation.


## Parsing:
- We need to parse the message from the client
- A buffer size of 512 bytes is used so as to hold 1 full message (where last two are CR-LF)

## Nick name changes (Not allowed)
## Servers:
-  The server forms the backbone of IRC, providing a point to which clients may connect to to talk to each other
									SERVER
									/  |  \
								   /   |   \
								  /    |    \
								 /     |     \
								/      |      \
							client1  ......  clientN
## Channels:
- 


## Servers:
- Servers are uniquely identified by their name, which has a maximum length of sixty three (63) characters.

# The IRC Specification:
##  Register the user:
- The recommended order for a client to register is as follows:
	- PASS MESSAGE
	- NICK MESSAGE
	- USER MESSAGE
- Password message:
	- The PASS command is used to set a 'connection password'.  The password can and must be set before any attempt to register the connection is made.  Currently this requires that clients send a PASS command before sending the NICK/USER combination and servers *must* send a PASS command before any SERVER command.  The password supplied must match the one contained in the C/N lines (for servers) or I lines (for clients).  It is possible to send multiple PASS commands before registering but only the last one sent is used for verification and it may not be changed once registered.
	```
	Command: PASS
	Parameters: <password>
	Numeric Replies:
			ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
	Example:
			PASS secretpasswordhere
	```
- Nick message:
	- 

```
Command: NICK
Parameters: <nickname>
Numeric Replies:

           ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
           ERR_NICKNAMEINUSE               ERR_NICKCOLLISION

   Example:

   NICK Wiz                        ; Introducing new nick "Wiz".
```
