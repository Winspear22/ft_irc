# include "MyMsg.hpp"

MyMsg::MyMsg( void )
{
	std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}


MyMsg::MyMsg( Clients *client, std::string MyMsg )
{
	(void)client;
	(void)MyMsg;
	std::cout << GREEN << "MyMsg Constructor called." << NORMAL << std::endl;
	return ;
}

MyMsg::MyMsg( const MyMsg & copy )
{
	std::cout << "\033[0;33mMyMsg Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

MyMsg & MyMsg::operator=( MyMsg const & rhs )
{
	std::cout << "\033[0;34mMyMsg Copy assignment operator called." << NORMAL << std::endl;
	(void)rhs;
	return (*this);
}


MyMsg::~MyMsg( void )
{
	std::cout << RED << "MyMsg Destructor called." << NORMAL << std::endl;
	return ;
}
