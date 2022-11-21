// Server side C program to demonstrate HTTP Server programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	
	// Only this line has been changed. Everything is same.
	char *hello = "HTTP/1.1 200 OK\n\n<!DOCTYPE html><html>\n\
	<head>\n\
		<title>Page Title</title>\n\
	</head>\n\
	<body>\n\
		<h1>This is a Heading</h1>\n\
		<p>This is a paragraph.</p>\n\
	</body>\n\
	</html>";
	printf ("%zu", strlen(hello));
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}
	

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		
		char buffer[30000] = {0};
		valread = read( new_socket , buffer, 30000);
		printf("%s\n",buffer );
		write(new_socket , hello , strlen(hello));
		// int bytes_sent = send(new_socket, hello, strlen(hello), 0);
		// printf("%d \n", bytes_sent);
		printf("------------------Hello message sent-------------------");
		close(new_socket);
	}
	return 0;
}