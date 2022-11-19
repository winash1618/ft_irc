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
	- We connect then ask the client to provide server password.
	- If the client gives a wrong server password we close the connection else continue.
- Set a nick name:
- Set a user name:
- Send and recieve private messages using your reference client: 
	- I believe this communication is with the server.(i am not sure)
## Channel Things:
- Create a channel:
- Allow users to join the channel that is created.
- if a message send by the user in a channel then we have to forward it to every other client.
## operator (Surprise!)
- We must have operators:
- They have specific commands to use those are not mentioned.
## User data: (How to store user data? Vector, map, structure,)
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
