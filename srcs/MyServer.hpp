#ifndef MYSERVER_HPP
# define MYSERVER_HPP

# include "irc.hpp"

class MyServer
{
public:
    MyServer( int port, std::string password );
	MyServer( const MyServer & copy);
    MyServer & operator=( const MyServer & server );
    ~MyServer( void );

    void        SetPort( char *str );
    void        SetPassword( char *str );
	void		SetServerStatus( int ServerStatus );

    int         GetPort( void );
	int			GetServerStatus( void );
    std::string GetPassword( void );

	int			CreateSocketFd( void );
	int			SetSocketOptions( void );
	int			BindSocketFd( void );
	int			ListenToSockedFd( void );
	int			SetSocketFdToNonBlocking( void );

private:
    MyServer(/* ARG */);


    int         	_port;
    std::string 	_password;
	bool			_server_status;
	int				_socketfd;
	sockaddr_in		_sockadress;
};


# endif