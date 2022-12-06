#include "Message.hpp"

const std::string	Message::msgRecv(int sock, bool &close_conn, bool &chk)
{
	char buffer[512];
	int rc = recv(sock, buffer, sizeof(buffer), 0);
	if (rc < 0)
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
	buffer[rc] = '\0';
	return buffer;
}

void	Message::sendReply(int numeric, const std::string& from, User &user, const std::string &cmd)
{
	std::string msg = ":" + from + " ";
	std::stringstream ss;
	ss << numeric;
	msg += ss.str() + " " + user.getNickName() + " ";
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
			msg += user.getNickName() + ":" + user.getAwayMsg() + "\n";
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
		case RPL_TIME:
		{
			time_t now = time(0);
			char* dt = ctime(&now);
			std::string str(dt);
			msg += user.getHostName() + " " + str + "\n";
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
			msg += cmd + " :You cant kill a server!\n";
			break ;
		}
		case ERR_PASSWDMISMATCH:
		{
			msg += " :Password incorrect\n";
			break ;
		}
		case ERR_NOSUCHSERVER:
		{
			msg += user.getHostName() +" :No such server\n";
			break ;
		}
		default:
		{
			break ;
		}
	}
	std::cout << msg << std::endl;
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