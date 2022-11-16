#ifndef MYSERVER_HPP
# define MYSERVER_HPP

# include "irc.hpp"

class MyServer
{
public:
    MyServer(/* ARG */);
    //MyServer( std::string port, std::string password );
    MyServer( const MyServer & copy);
    ~MyServer( void );
    MyServer & operator=( const MyServer & server );
    //std::ostream & operator<<( std::ostream & o );

    void        SetPort( char *str );
    void        SetPassword( char *str );

    int         GetPort( void );
    std::string GetPassword( void );

private:
    int         _port;
    std::string _password;
};

std::ostream & operator<<( std::ostream & o, MyServer rhs );

# endif