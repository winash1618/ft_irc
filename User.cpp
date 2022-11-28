#include "User.hpp"


User::User(): registered(false), timeout(20000) {

}

User::~User() {
	
}

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

const std::string	User::getPassword() const
{
	return (this->password);
}

void	User::setPassword(std::string password)
{
	this->password = password;
}

const std::string	User::getHostName() const
{
	return (this->hostname);
}

void	User::setHostName(std::string hostname)
{
	this->hostname = hostname;
}

const std::string	User::getIdent() const
{
	return (this->ident);
}

void	User::setIdent(std::string ident)
{
	this->ident = ident;
}

<<<<<<< HEAD
bool	User::getRegistered() const
{
	return (this->registered);
}

void	User::setRegistered(bool registered)
{
	this->registered = registered;
}

int	User::getTimeout() const
{
	return (this->timeout);
}

void	User::setTimeout(int timeout)
{
	this->timeout = timeout;
=======
bool	User::isOperator() const
{
	if (this->mode == "+o")
		return true;
	return false;
}

void	User::setUserMode(std::string mode)
{
	this->mode = mode;
>>>>>>> d30e483888fef4229611ecdf8449c8d642d34694
}