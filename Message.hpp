#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <errno.h>
#include "Numerics.hpp"
#include "User.hpp"
#include <sstream>
#include <sys/socket.h>
#include <ctime>

class Message {
private:
	std::string msg;
public:
	int					parseMessage(std::string msg);
	const std::string	msgRecv(int sock, bool& close_conn, bool &chk);
	void				sendReply(int numeric, const std::string& from, User& user, const std::string &cmd="");
	void				sendMessage(User &user, const std::string &msg);
	void				setMessage(const std::string &msg);
	const std::string	getNthWord(std::string s, std::size_t n);
	class MessageError : public std::exception
	{
	private:
		std::string _msg;
	public:
		MessageError(const std::string& msg) : _msg(msg){}
		~MessageError() throw() {}
		const char* what() const throw()
		{
			return _msg.c_str();
		}
	}; 
};

#endif