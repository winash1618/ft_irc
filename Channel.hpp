#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include "User.hpp"
#include <vector>

class User;

class Channel {
private:
	std::string 		name;
	std::vector<User*>	users;
public:
	Channel(const std::string &name);
	~Channel();
	const std::string	getChannelName() const;
	void				addUser(User *user);
	bool				userExists(const std::string &nickname);
	void				sendMessage(const std::string &from, const std::string &msg);
};

#endif