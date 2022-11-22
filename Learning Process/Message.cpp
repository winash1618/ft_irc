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

void	Message::sendReply(int numeric, std::string& from, User &user)
{
	std::string msg = ":" + from + " ";
	std::stringstream ss;
	ss << numeric;
	msg += ss.str() + " " + user.getNickName() + "!" + user.getUserName() + "@" + "localhost kabusitt :You are now logged in as kabusitt\n";
	send(user.getSocket(), msg.c_str(), msg.length(), 0);
	(void)numeric;
}

int	Message::parseMessage(std::string msg)
{
	std::string command = msg.substr(0, msg.find(" "));
	if (command == "JOIN")
		return (JOIN);
	else if (command == "NICK")
		return (NICK);
	else if (command == "PASS")
		return (PASS);
	else if (command == "USER")
		return (USER);
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