#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

#include <string>
#include "Client.hpp"
class ACommand
{
	public:
		ACommand();
		ACommand(const ACommand &copyMe);
		virtual ~ACommand();
		ACommand&		operator = (const ACommand &copyMe);
	

	// protected:
		virtual const char*	handleRequest(Client & client, std::string arg) = 0;
		virtual const char*	parseArgument(std::string arg) = 0;
		virtual const char*	action(Client & client, std::string arg) = 0;
		// add protected elements here

	private:
		// add private elements here
};

#endif