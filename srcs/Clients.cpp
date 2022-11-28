# include "Clients.hpp"

Clients::Clients( void )
{
	std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}


Clients::Clients( int ClientFd, struct sockaddr_in New_Address, std::string ServerName )
{
	this->_ClientFd = ClientFd;
	this->_New_Address = New_Address;
	this->_ServerName = ServerName;
	std::cout << GREEN << "Clients Constructor called." << NORMAL << std::endl;
	return ;
}

Clients::Clients( const Clients & copy )
{
	std::cout << "\033[0;33mClients Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

Clients & Clients::operator=( Clients const & rhs )
{
	std::cout << "\033[0;34mClients Copy assignment operator called." << NORMAL << std::endl;
	if ( this != &rhs )
    {
	    this->_ClientFd = rhs._ClientFd;
	    this->_New_Address = rhs._New_Address;
	    this->_ServerName = rhs._ServerName;
    }
	return (*this);
}


Clients::~Clients( void )
{
	std::cout << RED << "Clients Destructor called." << NORMAL << std::endl;
	return ;
}