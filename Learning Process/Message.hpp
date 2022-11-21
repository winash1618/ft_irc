#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <errno.h>
#include <sys/socket.h>

class Message {
private:
	std::string msg;
public:
	const std::string	parseMessage() const;
	const	std::string msgRecv(int, bool&);
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