#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Channel
{
	public:
		Channel();
		Channel(const Channel &copyMe);
		~Channel();
		Channel&		operator = (const Channel &copyMe);
	
	protected:
		// add protected elements here

	private:
		// add private elements here
};

#endif
