#ifndef MYSERVER_HPP
# define MYSERVER_HPP

# include "irc.hpp"
# include "Clients.hpp"

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
	
	void		AcceptClientsConnections( void );
	void		set_pollfd( std::vector<struct pollfd> &fds );
	void		serv_accept(std::vector<pollfd> &fds);

	
	
	void		SelectClients( void );
	void		RecvAndSend( void );

private:
    MyServer(/* ARG */);


    int         	_port;
    std::string 	_password;
	bool			_server_status;
	int				_socketfd;
	int				_acceptsocket;
	sockaddr_in		_sockadress;
	std::map<Clients*, int> _clients_list;
};


# endif
