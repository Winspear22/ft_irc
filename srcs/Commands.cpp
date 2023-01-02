# include "Commands.hpp"

Commands::Commands( void )
{
	std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}

Commands::Commands( const Commands & copy )
{
	std::cout << "\033[0;33mCommands Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

Commands & Commands::operator=( Commands const & rhs )
{
	std::cout << "\033[0;34mCommands Copy assignment operator called." << NORMAL << std::endl;
	(void)rhs;
	return (*this);
}

Commands::~Commands( void )
{
	std::cout << RED << "Commands Destructor called." << NORMAL << std::endl;
	return ;
}
