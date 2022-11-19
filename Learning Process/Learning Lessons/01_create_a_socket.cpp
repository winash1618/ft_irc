#include<stdio.h>
#include<sys/socket.h> 
#include <iostream>
// https://www.binarytides.com/socket-programming-c-linux-tutorial/
int main(int argc , char *argv[])
{
	int socket_desc;
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}

	std::cout << "socket file descriptor: " << socket_desc << std::endl;
	
	return 0;
}
// man socket
// int socket (int domain, int type, int protocol)
// Domain: It is the type of ip of address family used for communication.
// Given below are the most commonly used ip address family.
// 1. AF_INET6     IPv6 Internet protocols
// 2. AF_INET      IPv4 Internet protocols
// type: communication semantics means
// mainly used:
// 1. SOCK_STREAM: TCP(reliable, connection oriented)
// 2. SOCK_DGRAM: UDP(unreliable, connectionless)
// protocol: normally 0.