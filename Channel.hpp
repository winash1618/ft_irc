#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include "User.hpp"
#include <vector>

class User;

class Channel {
private:
	std::string name;
	std::vector<User*>	users;
public:
	const std::string	getChannelName() const;
};

#endif