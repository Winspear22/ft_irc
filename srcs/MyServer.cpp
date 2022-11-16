# include "MyServer.hpp"

MyServer::MyServer( void )
{
	std::cout << GREEN << "MyServer Constructor called." << NORMAL << std::endl;
	return ;
}


/*MyServer::MyServer( std::string port, std::string password )
{
	std::cout << GREEN << "MyServer Constructor called." << NORMAL << std::endl;
    
    
    this->_port = atoi(port.c_str());
    this->_password = password;
    std::cout << this->_port << std::endl;
    std::cout << this->_password << std::endl;
    return ;
}*/

MyServer::MyServer( const MyServer & copy )
{
	std::cout << "\033[0;33mMyServer Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

MyServer::~MyServer( void )
{
	std::cout << RED << "MyServer Destructor called." << NORMAL << std::endl;
	return ;
}

MyServer & MyServer::operator=( MyServer const & rhs )
{
	std::cout << "\033[0;34mMyServer Copy assignment operator called." << NORMAL << std::endl;
	if ( this != &rhs )
    {
        return (*this);
    }
	return (*this);
}

void        MyServer::SetPort( char *str )
{
	    this->_port = atoi(str);
		//std::cout << *this << std::endl;
}

void        MyServer::SetPassword( char *str )
{
	    this->_password = str;
		//std::cout << *this << std::endl;
}


int     MyServer::GetPort( void )
{
    return (this->_port);
}

std::string     MyServer::GetPassword( void )
{
    return (this->_password);
}

/*std::ostream & MyServer::operator<<( std::ostream & o )
{
    o << "The IRC_Server port is : " << BLUE << this->GetPort() << NORMAL << std::endl;
    o << "The IRC_Server password is : " << PURPLE << this->GetPassword() << NORMAL << std::endl;
    return (o);
}*/

std::ostream & operator<<( std::ostream & o, MyServer rhs )
{
	o << "The IRC_Server port is : " << BLUE << rhs.GetPort() << NORMAL << std::endl;
    std::cout << rhs.GetPort();
	o << "The IRC_Server password is : " << PURPLE << rhs.GetPassword() << NORMAL << std::endl;
        std::cout << rhs.GetPassword();

	return (o);
}