#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

#include <string>

class ACommand
{
	public:
		ACommand();
		ACommand(const ACommand &copyMe);
		virtual ~ACommand();
		ACommand&		operator = (const ACommand &copyMe);
	

	protected:
		virtual char*	handleRequest() = 0;
		virtual char*	parseArgument() = 0;
		virtual char*	action() = 0;
		// add protected elements here

	private:
		// add private elements here
};

#endif