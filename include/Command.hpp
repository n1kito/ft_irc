#ifndef COMMAND_HPP
# define COMMAND_HPP

class Command
{
	public:
		Command();
		Command(const Command &copyMe);
		~Command();
		Command&		operator = (const Command &copyMe);
	
	protected:
		// add protected elements here

	private:
		// add private elements here
};

#endif
