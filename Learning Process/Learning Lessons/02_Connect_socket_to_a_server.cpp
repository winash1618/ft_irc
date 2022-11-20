/**
 * @file 02_Connect_socket_to_a_server.cpp
 * @author Muhammed Asharaf (mahdimam@gmail.com)
 * @brief https://www.binarytides.com/socket-programming-c-linux-tutorial/
 * @version 0.1
 * @date 2022-11-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// To connect to a remote server we need to do a couple of things. 
// First is to create a sockaddr_in structure with proper values.

// struct sockaddr_in server;

// Have a look at the structure

// IPv4 AF_INET sockets:
// struct sockaddr_in {
//     short            sin_family;   // e.g. AF_INET, AF_INET6
//     unsigned short   sin_port;     // e.g. htons(3490)
//     struct in_addr   sin_addr;     // see struct in_addr, below
//     char             sin_zero[8];  // zero this if you want to
// };

// struct in_addr {
//     unsigned long s_addr;          // load with inet_pton()
// };

// struct sockaddr {
//     unsigned short    sa_family;    // address family, AF_xxx
//     char              sa_data[14];  // 14 bytes of protocol address
// };

// The sockaddr_in has a member called sin_addr of type in_addr 
// which has a s_addr which is nothing but a long. 
// It contains the IP address in long format.

// Function inet_addr is a very handy function to convert an 
// IP address to a long format. This is how you do it :

// server.sin_addr.s_addr = inet_addr("74.125.235.20");

// So you need to know the IP address of the remote server you are connecting to. 
// Here we used the ip address of google.com as a sample. 
// A little later on we shall see how to find out the ip address of a given domain name.

// The last thing needed is the connect function. 
// It needs a socket and a sockaddr structure to connect to. 
// Here is a code sample.

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr

int main(int argc , char *argv[])
{
	int socket_desc;
	struct sockaddr_in server;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
		
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 80 );

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	
	puts("Connected");
	return 0;
}

// Connect function:
// man connect
// int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
