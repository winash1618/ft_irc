#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sstream>
#include <fcntl.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <exception>
#include <vector>
#include "Message.hpp"
#include "User.hpp"

#define SERVER_PORT 6667

class User;

class Server {
private:
	int					sock;
	int					port;
	std::string			password;		
  	struct sockaddr_in6	addr;
	std::vector<User*>	users;
	Message				message;
  	struct pollfd		fds[200];
	int					nfds;
	int					user_sd;
	bool				close_conn;
	bool				server_running;
	bool				reorder_fds;
public:
	Server(std::string, std::string);
	~Server();
	void		socketCreate();
	int			socketAccept();
	void		socketOptionsInit();
	void		socketBind();
	void		socketListen();
	void		makeNonBlocking();
	void		printHostname();
	void		pollInit();
	void		pollFdInit();
	void		loopFds();
	void		reorderFds();
	bool		getServerRunning() const;
	int			getSocket() const;
	int			getNumberOfFds() const;
	bool		getReorderFds() const;
	class ServerError : public std::exception
	{
	private:
		std::string _msg;
	public:
		ServerError(const std::string& msg) : _msg(msg){}
		~ServerError() throw() {}
		const char* what() const throw()
		{
			return _msg.c_str();
		}
	}; 
};

#endif