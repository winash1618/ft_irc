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
	printf("Waiting on poll()...\n");
    int rc = poll(this->fds, this->nfds, -1);

    if (rc < 0)
    {
		std::string errMsg = strerror(errno);
		errMsg += " poll() failed.\n";
		close(this->sock);
		throw Server::ServerError(errMsg);
    }

    if (rc == 0)
    {
		std::string errMsg = strerror(errno);
		errMsg += " poll() timed out.  End program.\n";
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

void	Server::loopFds()
{
	int current_size = this->nfds;
	for (int i = 0; i < current_size; i++)
    {
      if(this->fds[i].revents == 0)
        continue;

      if(this->fds[i].revents != POLLIN && this->fds[i].revents != 17)
      {
        printf("  Error! revents = %d\n", this->fds[i].revents);
        this->server_running = true;
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
				std::string msg = this->message.msgRecv(fds[i].fd, close_conn);
				if (msg == "break")
					break ;
				std::cout << msg << std::endl;
				int	cmd = this->message.parseMessage(msg);
				switch (cmd) {
					case MSG: {
						std::string	name = message.getNthWord(msg, 2)[0] == '#';
						if (name[0] == '#')
						{
							for (std::vector<Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
							{
								if ((*it)->getChannelName() == name)
								{
									(*it)->
								}
							}
						}
						// else
							// send message to person
							// user who sent message users[i - 1]->getNickName();
							// 2nd parameter will be the user to send to so you have to search in member variable users to find it.
						break;
					}
					case JOIN: {
						break ;
					}
					case NICK: {
						users[i - 1]->setNickName(this->message.getNthWord(msg, 2));
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
							message.sendReply(RPL_LOGGEDIN, this->hostname, *(users[i - 1]));
						}
						break ;
					}
					case USER: {
						users[i - 1]->setUserName(this->message.getNthWord(msg, 2));
						std::string hostname = this->message.getNthWord(msg, 3);
						if (hostname == "0")
							message.sendMessage(*(users[i - 1]), ":" + this->hostname + " NOTICE * :*** Could not resolve your hostname: Domain not found; using your IP address ("+ users[i - 1]->getHostName() +") instead.\n");
						else
							users[i - 1]->setHostName(hostname);
						users[i - 1]->setIdent("~" + users[i - 1]->getNickName());
						message.sendMessage(*(users[i - 1]), ":" + this->hostname + " NOTICE kazem :*** Could not find your ident, using " + users[i - 1]->getIdent() + " instead.\n");
						message.sendMessage(*(users[i - 1]), ":" + this->hostname +" CAP kabusitt ACK :sasl\n");
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