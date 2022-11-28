#include "Server.hpp"


Server::Server(std::string port, std::string password):	password(password), nfds(1), user_sd(-1), server_running(true) {
	std::istringstream(port) >> this->port;
	char hostbuffer[256] = {0};
	gethostname(hostbuffer, sizeof(hostbuffer));
	this->hostname = hostbuffer;
}

Server::~Server()
{
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

void	Server::commandRun()
{
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
				message.sendMessage(*(users[i - 1]), "ERROR :Closing link: (" + users[i - 1]->getNickName() + "@" + users[i - 1]->getHostName() + ") [Registration timeout]\n");
				close(fds[i].fd);
				users.erase(users.begin() + (i - 1));
				fds[i].fd = -1;
				reorder_fds = true;
				break ;
			}
		}
		if(this->fds[i].revents == 0)
			continue;

		if(this->fds[i].revents != POLLIN && this->fds[i].revents != 17)
		{
			printf("  Error! revents = %d\n", this->fds[i].revents);
			this->server_running = false;
			break;
		}
		if (this->fds[i].fd == this->sock)
		{
			printf("  Listening socket is readable\n");
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
				printf("  New incoming connection - %d\n", this->user_sd);
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
			printf("  Descriptor %d is readable\n", fds[i].fd);
			this->close_conn = false;
			do
			{
				try {
					bool chk = false;
					std::string msg = this->message.msgRecv(fds[i].fd, this->close_conn, chk);
					if (chk)
						break ;
					std::cout << msg << std::endl;
					int	cmd = this->message.parseMessage(msg);
					switch (cmd)
					{
						case MSG:
						{
							std::string	name = this->message.getNthWord(msg, 2);
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
									}
								}
							}
							// else
								// send message to person
								// user who sent message users[i - 1]->getNickName();
								// 2nd parameter will be the user to send to so you have to search in member variable users to find it.
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
							break ;
						}
						case NICK: {
							std::string nickname = this->message.getNthWord(msg, 2);
							if (users[i - 1]->getNickName().empty())
									users[i - 1]->setNickName(nickname);
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
							break ;
						}
						case PASS: {
							break ;
						}
						case PLAIN: {

							if (users[i - 1]->getPassword().empty())
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
							else if (users[i - 1]->getPassword().empty())
							{
								users[i - 1]->setPassword(message.getNthWord(msg, 2));
								users[i - 1]->setRegistered(true);
								message.sendReply(RPL_WELCOME, this->hostname, *(users[i - 1]));
								message.sendReply(RPL_YOURHOST, this->hostname, *(users[i - 1]));
								message.sendReply(RPL_CREATED, this->hostname, *(users[i - 1]));
							}
							break ;
						}
						case USER: {
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
							break ;
						}
						default:
						{
							message.sendReply(ERR_UNKNOWNCOMMAND, this->hostname, *(users[i - 1]), message.getNthWord(msg, 1));
							break ;
						}
					}
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
					users.erase(users.begin() + (i - 1));
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
			nfds--;
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