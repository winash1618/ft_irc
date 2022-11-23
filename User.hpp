#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>
#include "Channel.hpp"
#include <map>

class Channel;

class User {
private:
	std::string					nickname;
	std::string					username;
	std::string					hostname;
	std::string					password;
	int							socket;
	std::map<Channel*, bool>	channels;
public:
	const std::string	getNickName() const;
	int					getSocket() const;
	const std::string	getUserName() const;
	void				setUserName(const std::string username);
	const std::string	getPassword() const;
	void				setPassword(const std::string password);
	const std::string	getHostName() const;
	void				setHostName(const std::string hostname);
	void				setSocket(const int);
	void				setNickName(const std::string nickname);
	bool				isOperator(Channel channel) const;
};

#endif