#ifndef MYSERVER_HPP
# define MYSERVER_HPP

# include "irc.hpp"
# include "Clients.hpp"
# include "MyMsg.hpp"
# include "Channels.hpp"

class Clients;
class MyMsg;

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
	int			GetSocketFd( void );

	int			CreateSocketFd( void );
	int			SetSocketOptions( void );
	int			BindSocketFd( void );
	int			ListenToSockedFd( void );
	int			SetSocketFdToNonBlocking( int SocketFd );

	int			SelectClients( void );
	void		CreateClients( void );
//	void		RecvClientsMsg( int ClientsFd );
	void		CheckClientsAuthentification( std::string cmd, MyMsg *msg );
	void		ExecuteCommand(std::string cmd, MyMsg *msg);

	Clients		*GetClientsThroughName( std::string name );
	Clients		*GetClientsThroughSocketFd( int fd );

	std::vector<std::string> SplitByEndline(char *str, const char *delim);

	Channels	*GetChannelsByName( std::string ChannelName );
	//void		CreateChannels( Channels *ChannelCreated );
	Channels	*CreateChannels( std::string Channelname, Clients *client );
	void		SendMsgToAllInChannels( std::string msg_sent );


	/*A EFFACER A LA FIN DU TEST*/
	void	buf_to_cmd( int ClientFd );
	void	RecvClientsMsg( int ClientFd );

	/*==========================*/


	std::map<Clients*, int> 			_clients_list;
	std::map<Channels*, std::string>	channels_list;
	MyMsg								*new_msg;

private:
    MyServer(/* ARG */);


    int         	_port;
    std::string 	_password;
	bool			_server_status;
	int				_socketfd; // sert de socket qui va listen
	sockaddr_in		_sockadress;
	bool			_right_password_used;
	/*TEST*/
	int				_new_fd_nb; // nouvel utilisateur se connecte et cree une nouvelle socket IL FAUT CREER UNE FONCTION SPECIALE POUR
	int				_nb_of_clients;

	int				_fds_list;
	int				_maximum_fds;
	/*pour avoir la liste des commandes complete*/
	std::vector<std::string> _cmd_list;
	std::vector<std::string>::iterator _it_cmd;

};

void		SendMsgBackToClients( MyMsg ClientMsg, std::string Msg );
void		SendMsgBackWithPrefix( MyMsg ClientMsg, std::string Msg );

/*En dehors du scope de la classe car je l'utilise dans une autre classe et dans laquelle il n'y a pas l'instance IRC_Server*/
# endif
