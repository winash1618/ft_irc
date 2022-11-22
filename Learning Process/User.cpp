#include "User.hpp"

const std::string User::getNickName() const
{
	return this->nickname;
}

void User::setNickName(const std::string nickname)
{
	this->nickname = nickname;
}

int User::getSocket() const
{
	return this->socket;
}

void User::setSocket(int socket)
{
	this->socket = socket;
}

const std::string	User::getUserName() const
{
	return (this->username);
}

void	User::setUserName(std::string username)
{
	this->username = username;
}