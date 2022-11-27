#include "Message.hpp"

const std::string	Message::msgRecv(int sock, bool &close_conn)
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
		return "break";
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
		case RPL_USERPARAMS:
		{
			msg += ":USER <username> <unused> <unused> :<realname>\n";
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
		case ERR_NICKNAMEINUSE:
		{
			msg += cmd + " :Nickname is already in use\n";
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
	else if (command == "AUTHENTICATE")
	{
		if (getNthWord(msg, 2) == "PLAIN")
			return (PLAIN);
		else
			return (AUTHENTICATE);
	}
	else if (command == "PRIVMSG")
	{
		return (MSG);
	}
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
	std::cout << msg << std::endl;
	send(user.getSocket(), msg.c_str(), msg.length(), 0);
}