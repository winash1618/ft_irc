#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>

#define PORT "6667"
#define ADDRESS "127.0.0.1"
 
int main(int argc, char const* argv[])
{
	struct addrinfo *result;
	struct addrinfo info;
	struct hostent *server;
	int	sock;
	int	err;

	bzero(&info, sizeof(info));
	info.ai_family = PF_INET;
	info.ai_socktype = SOCK_STREAM;
	err = getaddrinfo(ADDRESS, PORT, &info, &result);
	if (err != 0)
	{
		std::cerr << gai_strerror(err) << std::endl;
		return (-1);
	}
	//Only a few options for now
	for (struct addrinfo *i = result; i != NULL; i = i->ai_next)
	{
		switch (i->ai_family)
		{
		case PF_INET:
		{
			sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
			if (sock == -1)
			{
				perror(NULL);
				return (-1);
			}
			if (connect(sock, i->ai_addr, i->ai_addrlen) == -1)
			{
				perror(NULL);
				return (-1);
			}
			while (1)
			{
				std::string getInput;
				char buf[100];
				std::getline(std::cin, getInput);
				send(sock, getInput.c_str(), getInput.length(), 0);
				recv(sock, buf, 100, 0);
				std::cout << buf << std::endl;
			}
			break;
		}
		case PF_INET6:
		{
			sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
			if (sock == -1)
			{
				perror(NULL);
				return (-1);
			}
			if (connect(sock, i->ai_addr, i->ai_addrlen) == -1)
			{
				perror(NULL);
				return (-1);
			}
			break;
		}
		}
	}
	freeaddrinfo(result);
	return (0);
}