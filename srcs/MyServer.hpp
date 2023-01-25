#ifndef MYSERVER_HPP
# define MYSERVER_HPP

# include "irc.hpp"

class Clients;
class MyMsg;
class Channels;

class MyServer
{
public:
    MyServer( int port, std::string password );
    ~MyServer( void );

	void		InitVariables( void );

	/*===========================================*/
	/*             GETTERS AND SETTERS           */
	/*--------------All the Setters--------------*/
    void        SetPort( char *str );
    void        SetPassword( char *str );
	void		SetServerStatus( int ServerStatus );
	void		SetCurrentClientsNb( int CurrentNb );
	int			SetUnavailableNickname(std::string nickname);
	/*-------------------------------------------*/
	/*--------------All the Getters--------------*/
    int         GetPort( void );
	int			GetServerStatus( void );
    std::string GetPassword( void );
	int			GetSocketFd( void );
	int			GetCurrentClientsNb( void );
	int			isUnavailableNickname(std::string nickame);
	Clients		*GetClientsThroughName( std::string name );
	Clients		*GetClientsThroughSocketFd( int fd );
	Channels	*GetChannelsByName( std::string ChannelName );
	/*-------------------------------------------*/
	/*===========================================*/

	/*===========================================*/
	/*            SERVER CREATION PHASE          */   
	/*-----------Socket Creation Process---------*/
	int			CreateSocketFd( void );
	int			SetSocketOptions( void );
	int			BindSocketFd( void );
	/*-----------Listening and NB Phase----------*/
	int			ListenToSockedFd( void );
	int			SetSocketFdToNonBlocking( int SocketFd );
	/*-------------------------------------------*/
	/*===========================================*/

	/*===========================================*/
	/*       CLIENTS CONNECTION MANAGEMENT       */
	/*-----------Clients Connection Phase--------*/
	int			SelectClients( void );
	void		CreateClients( void );
	/*-------Clients Authentification Phase------*/
	void		CheckClientsAuthentification( std::string cmd, MyMsg *msg );
	/*---------Recv and Send Clients Msgs--------*/
	void		RecvClientsMsg( int ClientFd );
	//SendMsgBackToClients  --- Function outside of class
	//SendMsgBackWithPrefix --- Function outside of class
	void		SendMsgToAllInChannels( std::string msg_sent );
	/*-------------Command Execution-------------*/
	void		ExecuteCommand(std::string cmd, MyMsg *msg);
	/*-------AFK Clients and Channels Mngmt------*/
	int			DeleteAFKClients( void );
	Channels	*CreateChannels( std::string Channelname, Clients *client );
	int			DeleteChannelsWithoutClients( void );
	/*--------------------------------------------*/
	/*============================================*/

	/*----------Server Shutdown Message----------*/
	void		MyServerDestructorMsg( void );
	/*---------------Miscellaneous---------------*/
	void						deleteUnavailableNickname( void );
	std::vector<std::string> 	SplitByEndline(char *str, const char *delim);


	MyMsg								*new_msg;
	std::map<Clients*, int> 			clients_list;
	std::map<Channels*, std::string>	channels_list;
	fd_set								ready_fds;
	fd_set								readfds;

private:
    MyServer(void);
	MyServer( const MyServer & copy);
    MyServer & operator=( const MyServer & server );

    int         						_port;
    std::string 						_password;
	bool								_server_status;
	int									_socketfd;
	sockaddr_in							_sockadress;
	bool								_right_password_used;

	int									_new_fd_nb;
	int									_nb_of_clients;
	int									_fds_list;
	int									_maximum_fds;
	std::vector<std::string> 			_cmd_list;
	std::vector<std::string>::iterator	_it_cmd;
	std::map<std::string, clock_t> 		_unavailable_nicknames;
};

void		SendMsgBackToClients( MyMsg ClientMsg, std::string Msg );
void		SendMsgBackWithPrefix( MyMsg ClientMsg, std::string Msg );

# endif