#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>
#include "Channel.hpp"
#include <vector>

class Channel;

class User {
private:
	std::string				nickname;
	std::string				username;
	std::string				hostname;
	std::string				ident;
	std::string				mode;
	std::string				password;
	int						socket;
	std::vector<Channel*>	channels;
public:
	const std::string	getNickName() const;
	void				setNickName(const std::string nickname);
	int					getSocket() const;
	void				setSocket(const int);
	const std::string	getUserName() const;
	void				setUserName(const std::string username);
	const std::string	getPassword() const;
	void				setPassword(const std::string password);
	const std::string	getHostName() const;
	void				setHostName(const std::string hostname);
	void				setOperatorStatus(bool status);
	const std::string	getIdent() const;
	void				setIdent(const std::string ident);
	// bool				isOperator(Channel channel) const;
	bool				isOperator() const;
	void				setUserMode(std::string mode);
};

#endif