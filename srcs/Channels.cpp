# include "Channels.hpp"

Channels::Channels( void )
{
	std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}


Channels::Channels( void )
{
	std::cout << GREEN << "Channels Constructor called." << NORMAL << std::endl;
	return ;
}

Channels::Channels( const Channels & copy )
{
	std::cout << "\033[0;33mChannels Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

Channels & Channels::operator=( Channels const & rhs )
{
	std::cout << "\033[0;34mChannels Copy assignment operator called." << NORMAL << std::endl;
	(void)rhs;
	return (*this);
}


Channels::~Channels( void )
{
	std::cout << RED << "Channels Destructor called." << NORMAL << std::endl;
	return ;
}
