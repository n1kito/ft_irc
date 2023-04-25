#include "ft_irc.hpp"
#include "Server.hpp"
#include <cerrno>

/*
Classes:
	- server
	- client
	- command
	- channel
*/

int	main(int argc, char* argv[])
{
	try
	{
		errno = 0;
		// int			port = 0;
		// std::string	password;
		// checkArguments(argc, argv, port, password);

		if (argc != 3)
			throw std::invalid_argument("Wrong argument count.\nUsage: \033[3m./ircserv <port> <password>\033[0m");
		int port = 6667;
		Server	server(port, argv[2]);
		return (0);	
	}
	catch (const std::exception& e)
	{
		std::cout	<< BRED << "Error" << RESET << ": "
					<< e.what() << "." << std::endl;
		if (errno != 0)
			std::cout	<< BRED << "Description" << RESET << ": "
						<< strerror(errno) << " (" << static_cast<int>(errno) << ")"
						<< std::endl;
		return (1);
	}
}