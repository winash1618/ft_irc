#include "Channel.hpp"

Channel::Channel(void)
{
}

Channel::Channel(std::string name, std::string key, User &user) : ch_name(name), ch_key(key)
{
	this->users.push_back(&user);
	this->operators.push_back(&user);
}

Channel::~Channel(void)
{
}

const std::string	Channel::getChannelName() const
{
	return this->ch_name;
}

void	Channel::joinChannel(std::string key, User &user)
{
	if(key == this->ch_key)
		this->users.push_back(&user);
	else
		std::cout << "Channel key entered is wrong" << std::endl;
}


