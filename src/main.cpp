#include "ft_irc.hpp"
#include "Server.hpp"
#include <cerrno>
#include "ACommand.hpp"

int running = 1;
int	main(int argc, char* argv[])
{
	signal(SIGINT, signalHandler);

	try
	{
		errno = 0;
		// TODO: uncomment this before push
		// branch mj-fixing-password-parsing
		// int			port = 0;
		// std::string	password;
		// checkArguments(argc, argv, port, password);

		if (argc != 3)
			throw std::invalid_argument("Wrong argument count.\nUsage: \033[3m./ircserv <port> <password>\033[0m");
		int port = 6667;
		Server	server(port, argv[2], "pouetMania");
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