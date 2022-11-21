#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>
#include "Channel.hpp"
#include <map>

class Channel;

class User {
private:
	std::string nickname;
	int         socket;
	std::map<Channel*, bool> channels;
public:
	const std::string	getNickName() const;
	int					getSocket() const;
	void				setSocket(const int);
	void				setNickName(const std::string);
	bool				isOperator(Channel channel) const;
};

#endif