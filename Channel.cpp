#include "Channel.hpp"

Channel::Channel(const std::string &name) : name(name)
{

}

Channel::~Channel()
{
	
}

const std::string Channel::getChannelName() const
{
	return this->name;
}

void	Channel::sendMessage(const std::string &from, const std::string &msg)
{
	std::string sendMsg = ":" + from + " " + msg + "\n";
	for (std::vector<User*>::iterator it = this->users.begin(); it != this->users.end(); it++)
	{
		if ((*it)->getNickName() != from)
			send((*it)->getSocket(), sendMsg.c_str(), sendMsg.length(), 0);
	}
}

bool	Channel::userExists(const std::string &nickname)
{
	for (std::vector<User*>::iterator it = this->users.begin(); it != this->users.end(); it++)
	{
		if ((*it)->getNickName() == nickname)
			return (true);
	}
	return (false);
}

void	Channel::addUser(User *user)
{
	this->users.push_back(user);
}