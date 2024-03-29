#include "Server.hpp"


Server::Server(std::string port, std::string password):	password(password), nfds(1), user_sd(-1), server_running(true), reorder_fds(false) {
	std::istringstream(port) >> this->port;
	if (this->port < 1024 || this->port > 65535)
		throw Server::ServerError("Invalid port number");
	char hostbuffer[256] = {0};
	gethostname(hostbuffer, sizeof(hostbuffer));
	this->hostname = hostbuffer;
}

Server::~Server()
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		delete this->users[i];
	}
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		delete this->channels[i];
	}
	for (int i = 0; i < nfds; i++)
	{
		if(this->fds[i].fd >= 0)
		close(this->fds[i].fd);
	}
}

void	Server::socketCreate()
{
	this->sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (this->sock < 0)
	{
		std::string errMsg = strerror(errno);
		errMsg += " socket creation() failed\n";
		throw Server::ServerError(errMsg);
	}
}

void	Server::socketListen()
{
	int rc = listen(this->sock, 32);
	if (rc < 0)
	{
		std::string errMsg = strerror(errno);
		errMsg += " listen() failed\n";
		close(this->sock);
		throw Server::ServerError(errMsg);
	}
}

void	Server::printHostname()
{
	std::cout << "Hostname: " + this->hostname << std::endl;
}

int	Server::getNumberOfFds() const
{
	return this->nfds;
}

void	Server::pollFdInit()
{
	memset(this->fds, 0 , sizeof(fds));
	this->fds[0].fd = this->sock;
	this->fds[0].events = POLLIN;
}

void	Server::pollInit()
{
    int rc = poll(this->fds, this->nfds, 1000);

    if (rc < 0)
    {
		std::string errMsg = strerror(errno);
		errMsg += " poll() failed.\n";
		close(this->sock);
		throw Server::ServerError(errMsg);
    }
}

void	Server::makeNonBlocking()
{
	int rc = fcntl(this->sock, F_SETFL, O_NONBLOCK);
	if (rc < 0)
	{
		std::string errMsg = strerror(errno);
		errMsg += " fcntl() failed";
		close(this->sock);
		throw Server::ServerError(errMsg);
	}
}

int	Server::socketAccept()
{
	unsigned int size = sizeof(this->client);
	memset(&this->client, 0, sizeof(this->client));
	this->user_sd = accept(this->sock, (struct sockaddr *)&(this->client), &size);
	if (this->user_sd < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			std::string errMsg = strerror(errno);
			errMsg += " accept() failed";
			this->server_running = false;
			throw Server::ServerError(errMsg);
		}
		return (1);
	}
	return (0);
}

bool	Server::nickNameExists(std::string nickname) {
	for (std::vector<User*>::iterator it = this->users.begin(); it != this->users.end(); it++)
	{
		if ((*it)->getNickName() == nickname)
			return true;
	}
	return false;
}

void	Server::privMsgCommand(const std::string &msg, int i)
{
	std::string	name = this->message.getNthWord(msg, 2);
	bool found = false;
	if (name[0] == '#')
	{
		for (std::vector<Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
		{
			if ((*it)->getChannelName() == name)
			{
				if ((*it)->userExists(users[i - 1]->getNickName()))
					(*it)->sendMessage(users[i - 1]->getNickName(), msg);
				else
					this->message.sendReply(ERR_CANNOTSENDTOCHAN, this->hostname, *(users[i - 1]), name);
				found = true;
			}
		}
		if (!found)
			this->message.sendReply(ERR_NOSUCHCHANNEL, this->hostname, *(users[i - 1]), name);
	}
	else
	{
		std::stringstream stream(msg);
		size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
		if (!nickNameExists(message.getNthWord(msg, 2)))
		{
			message.sendReply(ERR_NOSUCHNICK, this->hostname, *(users[i - 1]), message.getNthWord(msg, 2));
		}
		else if (size < 3 && nickNameExists(message.getNthWord(msg, 2)))
		{
			message.sendReply(ERR_NOTEXTTOSEND, this->hostname, *(users[i - 1]));
		}
		else
		{
			for (std::vector<User*>::iterator it = this->users.begin(); it != this->users.end(); it++)
			{
				if ((*it)->getNickName() == name)
				{
					std::string sendMsg = ":" + users[i - 1]->getNickName() + " " + msg + "\n";
					send((*it)->getSocket(), sendMsg.c_str(), sendMsg.length(), 0);
					if ((*it)->getIsAway() == true)
					{
						std::string away;
						away = (*it)->getNickName() + " " + (*it)->getAwayMsg();
						message.sendReply(RPL_AWAY, this->hostname, *(users[i - 1]), away);
					}

				}
			}
		}
	}
}

void	Server::noticeCommand(const std::string &msg, int i)
{
	std::string	name = this->message.getNthWord(msg, 2);
	std::stringstream stream(msg);
	size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
	if (!nickNameExists(message.getNthWord(msg, 2)))
	{
		message.sendReply(ERR_NOSUCHNICK, this->hostname, *(users[i - 1]), message.getNthWord(msg, 2));
	}
	else if (size < 3 && nickNameExists(message.getNthWord(msg, 2)))
	{
		message.sendReply(ERR_NOTEXTTOSEND, this->hostname, *(users[i - 1]));
	}
	else
	{
		for (std::vector<User*>::iterator it = this->users.begin(); it != this->users.end(); it++)
		{
			if ((*it)->getNickName() == name)
			{
				std::string sendMsg = ":" + (*it)->getNickName() + " " + msg + "\n";
				send((*it)->getSocket(), sendMsg.c_str(), sendMsg.length(), 0);
			}
		}
	}
}

void	Server::joinCommand(const std::string &msg, int i)
{
	if (users[i - 1]->getRegistered())
	{
		std::string name = message.getNthWord(msg, 2);
		bool		found = false;
		for (std::vector<Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
		{
			if ((*it)->getChannelName() == name)
			{
				(*it)->addUser(users[i - 1]);
				found = true;
			}
		}
		if (!found)
		{
			Channel *channel = new Channel(name);
			channel->addUser(users[i - 1]);
			this->channels.push_back(channel);
		}
		this->message.sendMessage(*(users[i - 1]), ":" + users[i - 1]->getNickName() + "!" + users[i - 1]->getIdent() + "@" + users[i - 1]->getHostName() + " JOIN :" + name + "\n");
	}
	else
		message.sendReply(ERR_NOTREGISTERED, this->hostname, *(users[i - 1]), "JOIN");
}

void	Server::killCommand(const std::string &msg, int i)
{
	std::stringstream stream(msg);
	size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
	if (!users[i - 1]->isOperator())
	{
		message.sendReply(ERR_NOPRIVILEGES, this->hostname, *(users[i - 1]));
	}
	else if (size < 3)
	{
		message.sendReply(ERR_NEEDMOREPARAMS, this->hostname, *(users[i - 1]), "KILL");
	}
	else if (message.getNthWord(msg, 2) == hostname)
	{
		message.sendReply(ERR_CANTKILLSERVER, this->hostname, *(users[i - 1]));
	}
	else if (!nickNameExists(message.getNthWord(msg, 2)))
	{
		message.sendReply(ERR_NOSUCHNICK, this->hostname, *(users[i - 1]), message.getNthWord(msg, 2));
	}
	else if (message.getNthWord(msg, 2) == users[i - 1]->getNickName())
	{
		this->close_conn = true;
	}
	else
	{
		std::vector<User*>::iterator it = this->users.begin();
		size_t pos = 0;
		while ( it != this->users.end())
		{
			if ((*it)->getNickName() == message.getNthWord(msg, 2))
			{
				User *user = *it;
				users.erase(it);
				delete user;
				this->reorder_fds = true;
				close(fds[pos + 1].fd);
				fds[pos + 1].fd = -1;
				break;
			}
			pos++;
			it++;
		}
	}
}
void	Server::userCommand(const std::string &msg, int i)
{
	if (users[i - 1]->getUserName().empty())
	{
		std::string nickname = users[i - 1]->getNickName();
		users[i - 1]->setUserName(this->message.getNthWord(msg, 2));
		std::string hostname = this->message.getNthWord(msg, 3);
		if (hostname == "0")
			message.sendMessage(*(users[i - 1]), ":" + this->hostname + " NOTICE * :*** Could not resolve your hostname: Domain not found; using your IP address ("+ users[i - 1]->getHostName() +") instead.\n");
		else
			users[i - 1]->setHostName(hostname);
		if (nickname.find_first_not_of("0123456789") != std::string::npos) {
			users[i - 1]->setIdent("~" + users[i - 1]->getNickName().substr(0, 9));
			message.sendMessage(*(users[i - 1]), ":" + this->hostname + " NOTICE " + nickname + " :*** Could not find your ident, using " + users[i - 1]->getIdent() + " instead.\n");
			message.sendMessage(*(users[i - 1]), ":" + this->hostname +" CAP " + nickname + " ACK :sasl\n");
		}
	}
	else
	{
		std::stringstream stream(msg);
		size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
		if (size != 5)
		{
			message.sendReply(ERR_NEEDMOREPARAMS, this->hostname, *(users[i - 1]), "USER");
			message.sendReply(RPL_USERPARAMS, this->hostname, *(users[i - 1]));
		}
		else
		{
			message.sendReply(ERR_ALREADYREGISTRED, this->hostname, *(users[i - 1]));
		}
	}
}

void	Server::nickCommand(const std::string &msg, int i)
{
	std::stringstream stream(msg);
	size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
	std::string nickname = this->message.getNthWord(msg, 2);
	std::cout << "nickname: " << msg << "\n";
	std::cout << "size: " << size << "\n";
	if (nickname == ":" || size != 2)
	{
		message.sendReply(ERR_NEEDMOREPARAMS, this->hostname, *(users[i - 1]), "USER");
	}
	else if (users[i - 1]->getNickName().empty())
			users[i - 1]->setNickName(nickname);
	else if (users[i - 1]->getNickName() == nickname)
	{
		return ;
	}
	else if (nickNameExists(nickname))
		message.sendReply(ERR_NICKNAMEINUSE, this->hostname, *(users[i - 1]), nickname);
	else
	{
		if (nickname.find_first_not_of("0123456789") == std::string::npos)
			message.sendReply(ERR_ERRONEUSNICKNAME, this->hostname, *(users[i - 1]), nickname);
		else
		{
			if (!users[i - 1]->getRegistered())
			{
				users[i - 1]->setNickName(nickname);
				users[i - 1]->setIdent("~" + users[i - 1]->getNickName().substr(0, 9));
				message.sendMessage(*(users[i - 1]), ":" + this->hostname + " NOTICE " + nickname + " :*** Ident lookup timed out, using " + users[i - 1]->getIdent() + " instead.\n");
				message.sendMessage(*(users[i - 1]), ":" + this->hostname +" CAP " + nickname + " ACK :sasl\n");
			}
			else
			{
				message.sendMessage(*(users[i - 1]), ":" + users[i - 1]->getNickName() + " NICK " + nickname + "\n");
				users[i - 1]->setNickName(nickname);
			}
		}
	}
}

void	Server::sQuitCommand(const std::string &msg, int i)
{
	std::stringstream stream(msg);
	size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
	if (message.getNthWord(msg, 2) == hostname && size < 3)
	{
		message.sendReply(ERR_NEEDMOREPARAMS, this->hostname, *(users[i - 1]), "SQUIT");
	}
	else if (!users[i - 1]->isOperator())
	{
		message.sendReply(ERR_NOPRIVILEGES, this->hostname, *(users[i - 1]));
	}
	else if (message.getNthWord(msg, 2) != hostname)
	{
		message.sendReply(ERR_NOSUCHSERVER, this->hostname, *(users[i - 1]));
	}
	else
	{
		close(this->sock);
	}
}

void	Server::commandRun(const std::string &msg, int i)
{
	int	cmd = this->message.parseMessage(msg);
	switch (cmd)
	{
		case MSG:
		{
			privMsgCommand(msg, i);
			break;
		}
		case NOTICE:
		{
			noticeCommand(msg, i);
			break;
		}
		case PONG: {
			break;
		}
		case QUIT: {
			message.sendMessage(*(users[i - 1]), "ERROR :Closing link: (" + users[i - 1]->getNickName() + "@" + users[i - 1]->getHostName() + ") [Quit: ]\n");
			this->close_conn = true;
			break;
		}
		case JOIN: {
			joinCommand(msg, i);
			break ;
		}
		case NICK: {
			nickCommand(msg, i);
			break ;
		}
		case PASS: {
			std::stringstream stream(msg);
			size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
			if (size == 1)
			{
				message.sendReply(ERR_NEEDMOREPARAMS, this->hostname, *(users[i - 1]), "PASS");
				message.sendReply(RPL_USERPARAMS, this->hostname, *(users[i - 1]));
			}
			else if (message.getNthWord(msg, 2) != this->password)
			{
				message.sendReply(ERR_PASSWDMISMATCH, this->hostname, *(users[i - 1]));
				this->close_conn = true;
			}
			else if (users[i - 1]->getPassword().empty())
			{
				users[i - 1]->setPassword(this->password);
			}
			else
			{
				message.sendReply(ERR_ALREADYREGISTRED, this->hostname, *(users[i - 1]));
			}
			break ;
		}
		case PLAIN: {

			if (!users[i - 1]->getRegistered())
				message.sendMessage(*(users[i - 1]), "AUTHENTICATE +\n");
			break;
		}
		case AUTHENTICATE:
		{
			std::stringstream stream(msg);
			size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
			if (size == 1)
			{
				message.sendReply(ERR_NEEDMOREPARAMS, this->hostname, *(users[i - 1]), "AUTHENTICATE");
			}
			else if (!users[i - 1]->getRegistered())
			{
				users[i - 1]->setRegistered(true);
				message.sendReply(RPL_WELCOME, this->hostname, *(users[i - 1]));
				message.sendReply(RPL_YOURHOST, this->hostname, *(users[i - 1]));
				message.sendReply(RPL_CREATED, this->hostname, *(users[i - 1]));
				message.sendReply(RPL_MOTDSTART, this->hostname, *(users[i - 1]));
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "The commands that you have available");
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "/join <channel>");
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "/msg <nickname|channel>");
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "/oper <user> <passowrd>");
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "/away");
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "/nick <nickname>");
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "/squit <server> <comment>");
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "/away <message>");
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "/kill <nickname> <comment>");
				message.sendReply(RPL_MOTD, this->hostname, *(users[i - 1]), "/time [<server>]");
				message.sendReply(RPL_ENDOFMOTD, this->hostname, *(users[i - 1]));
			}
			break ;
		}
		case USER: {
			userCommand(msg, i);
			break ;
		}
		case OPER: {
			std::stringstream stream(msg);
			size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
			std::string default_password = "hello";
			if (size != 3)
			{
				message.sendReply(ERR_NEEDMOREPARAMS, this->hostname, *(users[i - 1]), "OPER");
			}
			else if (message.getNthWord(msg, 3) != default_password)
			{
				message.sendReply(ERR_PASSWDMISMATCH, this->hostname, *(users[i - 1]));
			}
			else
			{
				message.sendReply(RPL_YOUREOPER, this->hostname, *(users[i - 1]));
				users[i - 1]->setUserMode("o");
			}
			break;
		}
		case KILL: {
			killCommand(msg, i);
			break;
		}
		case SQUIT: {
			sQuitCommand(msg, i);
			break;
		}
		case AWAY: {
			std::stringstream stream(msg);
			size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
			if (size == 1)
			{
				users[i - 1]->setAwayMsg("");
				users[i - 1]->setIsAway(false);
				message.sendReply(RPL_UNAWAY, this->hostname, *(users[i - 1]));
			}
			else
			{
				std::size_t pos = msg.find("AWAY");
				std::string away_message = msg.substr(pos + 4);
				users[i - 1]->setAwayMsg(away_message);
				users[i - 1]->setIsAway(true);
				message.sendReply(RPL_NOWAWAY, this->hostname, *(users[i - 1]));
			}
			break;
		}
		case TIME:
		{
			std::stringstream stream(msg);
			size_t size = std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
			if (size == 1)
			{
				message.sendReply(RPL_TIME, this->hostname, *(users[i - 1]));
			}
			break;
		}
		default:
		{
			message.sendReply(ERR_UNKNOWNCOMMAND, this->hostname, *(users[i - 1]), message.getNthWord(msg, 1));
			break ;
		}
	}
}

void	Server::loopFds()
{
	int current_size = this->nfds;
	for (int i = 0; i < current_size; i++)
    {
		if (i != 0)
		{
			int timeout = users[i - 1]->getTimeout();
			if (timeout > 0)
				users[i - 1]->setTimeout(timeout - 1000);
			else if (timeout == 0 && !users[i - 1]->getRegistered())
			{
				message.setMessage("");
				message.sendMessage(*(users[i - 1]), "ERROR :Closing link: (" + users[i - 1]->getNickName() + "@" + users[i - 1]->getHostName() + ") [Registration timeout]\n");
				close(fds[i].fd);
				User *tmp = *(users.begin() + (i - 1));
				users.erase(users.begin() + (i - 1));
				delete tmp;
				fds[i].fd = -1;
				reorder_fds = true;
				break ;
			}
		}
		if(this->fds[i].revents == 0)
			continue;

		if(this->fds[i].revents != POLLIN && this->fds[i].revents != 17)
		{
			std::cout << "  Error! revents = " << this->fds[i].revents << std::endl;
			this->server_running = false;
			break;
		}
		if (this->fds[i].fd == this->sock)
		{
			std::cout << "  Listening socket is readable" << std::endl;
			do
			{
				try {
					if (socketAccept())
						break ;
				}
				catch (std::exception &exp)
				{
					std::cerr << exp.what() << std::endl;
					break ;
				}
				std::cout << "  New incoming connection - " << this->user_sd << std::endl;
				this->fds[this->nfds].fd = this->user_sd;
				this->fds[this->nfds].events = POLLIN;
				User *user = new User();
				user->setSocket(this->fds[this->nfds].fd);
				user->setHostName(inet_ntoa(this->client.sin_addr));
				this->users.push_back(user);
				this->nfds++;

			} while (this->user_sd != -1);
      	}
		else
		{
			this->close_conn = false;
			do
			{
				try {
					bool chk = false;
					std::string msg = this->message.msgRecv(fds[i].fd, this->close_conn, chk);
					if (chk)
						break ;
					if (!users[i - 1]->getIsPass() && message.getNthWord(msg, 1) != "PASS")
					{
						message.sendReply(ERR_PASSWDMISMATCH, this->hostname, *(users[i - 1]));
						this->close_conn = true;
					}
					else
					{
						users[i - 1]->setIsPass(true);
					}
					commandRun(msg, i);
				}
				catch (std::exception &exp)
				{
					std::cerr << exp.what() << std::endl;
					break ;
				}
			} while(true);

			if (this->close_conn)
			{
				close(fds[i].fd);
				if (i > 0)
				{
					User *tmp = *(users.begin() + (i - 1));
					users.erase(users.begin() + (i - 1));
					current_size--;
					delete tmp;
				}
				fds[i].fd = -1;
				reorder_fds = true;
			}
		}
    }
}

bool	Server::getReorderFds() const
{
	return this->reorder_fds;
}

bool Server::getServerRunning() const
{
	return this->server_running;
}

void	Server::reorderFds()
{
	this->reorder_fds = false;
	for (int i = 0; i < this->nfds; i++)
	{
		if (this->fds[i].fd == -1)
		{
			for(int j = i; j < this->nfds; j++)
				this->fds[j].fd = this->fds[j + 1].fd;
			i--;
			this->nfds--;
		}
	}
}

void	Server::socketOptionsInit()
{
	int on = 1;
	int rc = setsockopt(this->sock, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
	if (rc < 0)
	{
		std::string errMsg = strerror(errno);
		errMsg += " setsockopt() failed";
		close(this->sock);
		throw Server::ServerError(errMsg);
	}
}

void	Server::socketBind()
{
	memset(&this->addr, 0, sizeof(this->addr));
	this->addr.sin6_family      = AF_INET6;
	memcpy(&this->addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	this->addr.sin6_port        = htons(this->port);
	int rc = bind(this->sock,
				(struct sockaddr *)&this->addr, sizeof(this->addr));
	if (rc < 0)
	{
		std::string errMsg = strerror(errno);
		errMsg += " bind() failed";
		close(this->sock);
		throw Server::ServerError(errMsg);
	}
}

int	Server::getSocket() const
{
	return this->sock;
}

void	Server::sendMessageClient(const std::string &from, const std::string &msg)
{
	std::string sendMsg = ":" + from + " " + msg + "\n";
	for (std::vector<User*>::iterator it = this->users.begin(); it != this->users.end(); it++)
	{
		if ((*it)->getNickName() != from)
			send((*it)->getSocket(), sendMsg.c_str(), sendMsg.length(), 0);
	}
}