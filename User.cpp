#include "User.hpp"


User::User(): registered(false), timeout(1000000), is_pass(false), is_away(false) {
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

bool	User::getIsPass() const
{
	return this->is_pass;
}

void	User::setIsPass(bool is_pass)
{
	this->is_pass = is_pass;
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
}

bool	User::isOperator() const
{
	char ch = 'o';
	if (this->mode.find(ch) != std::string::npos)
		return true;
	return false;
}

void	User::setUserMode(std::string mode)
{
	char ch = 'o';
	if (this->mode.find(ch) == std::string::npos)
		this->mode += mode;
}

const std::string	User::getAwayMsg() const
{
	return (this->away_msg);
}

void	User::setAwayMsg(std::string away_msg)
{
	this->away_msg = away_msg;
}

bool	User::getIsAway() const
{
	return(this->is_away);
}

void	User::setIsAway(bool away)
{
	this->is_away = away;
}
