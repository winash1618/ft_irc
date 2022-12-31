#include "Message.hpp"
#include <cstring>

const std::string	Message::msgRecv(int sock, bool &close_conn, bool &chk)
{
	bool flag = false;// string to store received data
	std::string msg2;
	char buffer[512];
	static int rc;
	do
	{
		rc = recv(sock, buffer, sizeof(buffer), 0);
		if (rc == -1)
		{
			if (errno != EWOULDBLOCK)
			{
				close_conn = true;
				throw Message::MessageError("receive() failed");
			}
			chk = true;
			return "";
		}
		if (rc == 0)
		{
			close_conn = true;
			throw Message::MessageError("Connection closed\n");
		}
		if (rc > 0)
		{
			buffer[rc] = '\0';
			this->msg += buffer;
			if ((int)buffer[rc - 1] == 10)
			{
				msg2 = msg;
				msg = "";
				flag = true;
			}
		}
	} while ((int)buffer[rc - 1] != 10 && !flag);
	return msg2;
}

void	Message::setMessage(const std::string &msg)
{
	this->msg = msg;
}

void	Message::sendReply(int numeric, const std::string& from, User &user, const std::string &cmd)
{
	std::string msg = ":" + from + " ";
	std::stringstream ss;
	ss << numeric;
	msg += ss.str() + " " + (numeric == 464 ? "*" : user.getNickName()) + " ";
	switch (numeric)
	{
		case RPL_LOGGEDIN:
		{
			msg += user.getNickName() + "!" + user.getIdent() + "@" + user.getHostName() + " " + user.getNickName() + " :You are now logged in as " + user.getUserName() + "\n";
			break ;
		}
		case RPL_WELCOME:
		{
			msg += ":Welcome to the ircserv Network " + user.getNickName() + "!" + user.getIdent() + "@" + user.getHostName() + "\n";
			break ;
		}
		case RPL_AWAY:
		{
			msg += cmd + ":" + user.getAwayMsg() + "\n";
			break ;
		}
		case RPL_NOWAWAY:
		{
			msg += ":You have been marked as being away\n";
			break ;
		}
		case RPL_UNAWAY:
		{
			msg += ":You are no longer marked as being away\n";
			break ;
		}
		case RPL_YOUREOPER:
		{
			msg += ":You are now an IRC operator\n";
			break ;
		}
		case RPL_USERPARAMS:
		{
			msg += ":USER <username> <unused> <unused> :<realname>\n";
			break ;
		}
		case RPL_YOURHOST:
		{
			msg += ":Your host is " + from + ", running version ircserv1.1\n";
			break ;
		}
		case RPL_CREATED:
		{
			msg += ":This server was created 20/10/2022\n";
			break ;
		}
		case RPL_MOTDSTART:
		{
			msg += ":" + from + " message of the day\n";
			break ;
		}
		case RPL_MOTD:
		{
			msg += ": " + cmd + "\n";
			break ;
		}
		case RPL_ENDOFMOTD:
		{
			msg += ":End of message of the day.\n";
			break ;
		}
		case RPL_TIME:
		{
			time_t now = time(0);
			char* dt = ctime(&now);
			std::string str(dt);
			msg += from + " " + str + "\n";
			break ;
		}
		case ERR_NEEDMOREPARAMS:
		{
			msg += cmd + " :Not enough parameters\n";
			break ;
		}
		case ERR_UNKNOWNCOMMAND:
		{
			msg += cmd + " :Unknown command\n";
			break ;
		}
		case ERR_ALREADYREGISTRED:
		{
			msg += cmd + " :You may not reregister\n";
			break ;
		}
		case ERR_ERRONEUSNICKNAME:
		{
			msg += cmd + " :Erroneus nickname\n";
			break ;
		}
		case ERR_NICKNAMEINUSE:
		{
			msg += cmd + " :Nickname is already in use\n";
			break ;
		}
		case ERR_NOTREGISTERED:
		{
			msg += cmd + " :You have not registered\n";
			break ;
		}
		case ERR_CANNOTSENDTOCHAN:
		{
			msg += cmd + " :Cannot send to channel\n";
			break ;
		}
		case ERR_NOPRIVILEGES:
		{
			msg += cmd + " :Permission Denied- You're not an IRC operator\n";
			break ;
		}
		case ERR_CANTKILLSERVER:
		{
			msg += cmd + " :You cant kill a server!\n";
			break ;
		}
		case ERR_NOSUCHNICK:
		{
			msg += cmd + " :No such nick!\n";
			break ;
		}
		case ERR_PASSWDMISMATCH:
		{
			msg += " :Password incorrect\n";
			break ;
		}
		case ERR_NOSUCHSERVER:
		{
			msg += from +" :No such server\n";
			break ;
		}
		case ERR_NOTEXTTOSEND:
		{
			msg += " :No text to send\n";
			break ;
		}
		case ERR_NOSUCHCHANNEL:
		{
			msg += cmd + " :No such channel\n";
			break ;
		}
		default:
		{
			break ;
		}
	}
	send(user.getSocket(), msg.c_str(), msg.length(), 0);
}

int	Message::parseMessage(std::string msg)
{
	std::string command = getNthWord(msg, 1);

	if (command == "JOIN")
		return (JOIN);
	else if (command == "NICK")
		return (NICK);
	else if (command == "PASS")
		return (PASS);
	else if (command == "USER")
		return (USER);
	else if (command == "OPER")
		return (OPER);
	else if (command == "KILL")
		return (KILL);
	else if (command == "SQUIT")
		return (SQUIT);
	else if (command == "AWAY")
		return (AWAY);
	else if (command == "TIME")
		return (TIME);
	else if (command == "AUTHENTICATE")
	{
		if (getNthWord(msg, 2) == "PLAIN")
			return (PLAIN);
		else
			return (AUTHENTICATE);
	}
	else if (command == "PRIVMSG")
		return (MSG);
	else if (command == "NOTICE")
		return (NOTICE);
	else if (command == "PONG")
		return (PONG);
	else if (command == "QUIT")
		return (QUIT);
	return (0);
}

const std::string Message::getNthWord(std::string s, std::size_t n)
{
    std::istringstream iss (s);
    while(n-- > 0 && (iss >> s));
    return s;
}

void	Message::sendMessage(User &user, const std::string &msg)
{
	send(user.getSocket(), msg.c_str(), msg.length(), 0);
}