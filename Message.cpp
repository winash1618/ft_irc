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
	msg += ss.str() + " ";
	switch (numeric)
	{
		case RPL_LOGGEDIN:
		{
			msg += user.getNickName() + " " + user.getNickName() + "!" + user.getUserName() + "@" + user.getHostName() + " kabusitt :You are now logged in as " + user.getUserName() + "\n";
			break ;
		}
		case ERR_NEEDMOREPARAMS:
		{
			msg += cmd + " :Not enough parameters";
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
	else if (command == "AUTHENTICATE")
	{
		if (getNthWord(msg, 2) == "PLAIN")
			return (PLAIN);
		else
			return (AUTHENTICATE);
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
	send(user.getSocket(), msg.c_str(), msg.length(), 0);
}