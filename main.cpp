// https://www.ibm.coa.	m/docs/en/i/7.2?topic=designs-using-poll-instead-select
// https://stackoverflow.com/questions/34069937/c-socket-programming-client-server-connection-on-host-name
// https://www.tutorialspoint.com/c-program-to-display-hostname-and-ip-address

#include "Server.hpp"
#include "User.hpp"

int main (int argc, char *argv[])
{
	if (argc == 3)
	{
		for (size_t i = 0; i < strlen(argv[1]); i++)
		{
			if (!isdigit(argv[1][i]))
			{
				std::cerr << "Port must be a number" << std::endl;
				return (0);
			}
		}
			try {
				Server server(argv[1], argv[2]);
				server.socketCreate();
				server.socketOptionsInit();
				server.makeNonBlocking();
				server.socketBind();
				server.socketListen();
				server.printHostname();
				server.pollFdInit();
				do
				{
					server.pollInit();
					server.getNumberOfFds();
					server.loopFds();
					if (server.getReorderFds())
					{
						server.reorderFds();
					}

				} while (server.getServerRunning() == true);
			}
			catch (std::exception &exp)
			{
				std::cerr << exp.what() << std::endl;
			}
		return (0);
	}
	else
	{
		std::cerr << "Usage `ft_irc <port> <password>`" << std::endl;
	}
}
