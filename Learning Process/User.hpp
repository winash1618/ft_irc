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
	int							socket;
	std::map<Channel*, bool>	channels;
public:
	const std::string	getNickName() const;
	int					getSocket() const;
	const std::string	getUserName() const;
	void				setUserName(std::string);
	void				setSocket(const int);
	void				setNickName(const std::string);
	bool				isOperator(Channel channel) const;
};

#endif