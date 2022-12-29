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
	bool					registered;
	std::string				away_msg;
	std::string				password;
	int						socket;
	int						timeout;
	bool					is_pass;
	bool					is_away;
public:
	User();
	~User();
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
	bool				getRegistered() const;
	void				setRegistered(bool registered);
	bool				getIsPass() const;
	void				setIsPass(bool is_pass);
	int					getTimeout() const;
	void				setTimeout(int timeout);
	bool				isOperator() const;
	void				setUserMode(std::string mode);
	const std::string	getAwayMsg() const;
	void				setAwayMsg(std::string away_msg);
	bool				getIsAway() const;
	void				setIsAway(bool away);
};

#endif