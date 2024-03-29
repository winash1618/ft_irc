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
#include <iterator>
#include "Message.hpp"
#include "User.hpp"
#include "Numerics.hpp"

#define SERVER_PORT 6667

class User;

class Server {
private:
	int						sock; // socket descriptor
	int						port; // port number
	std::string				password; // server password
	std::string				hostname; // server hostname
  	struct sockaddr_in6		addr; // server address
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
	void		joinCommand(const std::string &msg, int i);
	void		nickCommand(const std::string &msg, int i);
	void		userCommand(const std::string &msg, int i);
	void		killCommand(const std::string &msg, int i);
	void		privMsgCommand(const std::string &msg, int i);
	void		noticeCommand(const std::string &msg, int i);
	void		sQuitCommand(const std::string &msg, int i);
	void		commandRun(const std::string &msg, int i);
	void		reorderFds();
	bool		getServerRunning() const;
	bool		nickNameExists(std::string nickname);
	int			getSocket() const;
	int			getNumberOfFds() const;
	bool		getReorderFds() const;
	void		sendMessageClient(const std::string &from, const std::string &msg);
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