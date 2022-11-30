#ifndef MYSERVER_HPP
# define MYSERVER_HPP

# include "irc.hpp"
# include "Clients.hpp"
# include "MyMsg.hpp"

class Clients;

class MyServer
{
public:
    MyServer( int port, std::string password );
	MyServer( const MyServer & copy);
    MyServer & operator=( const MyServer & server );
    ~MyServer( void );

	void		InitVariables( void );

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

	int			SelectClients( void );
	void		CreateClients( void );
	void		RecvClientsMsg( int ClientsFd );

	Clients		*GetClientsThroughName( std::string name );
	Clients		*GetClientsThroughSocketFd( int fd );

	std::map<Clients*, int> _clients_list;


private:
    MyServer(/* ARG */);


    int         	_port;
    std::string 	_password;
	bool			_server_status;
	int				_socketfd; // sert de socket qui va listen
	sockaddr_in		_sockadress;
	/*TEST*/
	int				_new_fd_nb; // nouvel utilisateur se connecte et cree une nouvelle socket IL FAUT CREER UNE FONCTION SPECIALE POUR
	int				_nb_of_clients;
};


# endif
