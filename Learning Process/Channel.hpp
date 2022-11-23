#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include "User.hpp"
#include <vector>

class User;
class Channel
{
	private:
		std::string ch_name;
		std::string ch_key;
		std::vector<User *>	users;
		std::vector<User *> operators;
	public:
		Channel();
		Channel(std::string name, std::string key, User &user);
		~Channel();
		const std::string	getChannelName() const;
		void				joinChannel(std::string key, User &user);
};

#endif