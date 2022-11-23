#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <arpa/inet.h>
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
#include <algorithm>
#include "Message.hpp"
#include "User.hpp"
#include "Numerics.hpp"

#define SERVER_PORT 6667

class User;

class Server {
private:
	int						sock;
	int						port;
	std::string				password;	
	std::string				hostname;
  	struct sockaddr_in6		addr;
	struct sockaddr_in		client;
	std::vector<User*>		users;
	std::vector<Channel*>	channels;
	Message					message;
  	struct pollfd			fds[200];
	int						nfds;
	int						user_sd;
	bool					close_conn;
	bool					server_running;
	bool					reorder_fds;
public:
	Server(std::string port, std::string password);
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