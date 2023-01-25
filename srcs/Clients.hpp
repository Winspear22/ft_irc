#ifndef CLIENTS_HPP
# define CLIENTS_HPP

# include "irc.hpp"
class MyMsg;

class Clients
{
public:
	Clients( int ClientFd, struct sockaddr_in _New_Address, std::string ServerName );
	~Clients( void );

	void			resetTime(void);
	std::string		_Nickname;
	std::string		_Hostname;
	std::string		_HostAdress;
	std::string		_ServerName;

	/*===========================================*/
	/*             GETTERS AND SETTERS           */
	/*--------------All the Setters--------------*/

	void			SetFirstMOTD( int motd );
	void			SetChanLim( int ChanLim );
	void			SetClientsLastPing( time_t ping);
	void			SetClientsMode( std::string Mode );
	void			SetClientsBuffer( std::string buff);
	void			SetClientsUnused( std::string Unused );
	void			SetServerName( std::string ServerName);
	void			SetClientsConnectionStatus( int status );
	void			SetClientsMessage( std::string msg_recv );
	void			SetClientsRealname( std::string realname );
	void			SetClientsNickname( std::string nickname );
	void			SetClientsHostname( std::string hostname );
	void			SetClientsUsername( std::string username );
	void			SetClientsHostAdress( std::string hostAdress );
	void			SetClientsConnectionPermission( int PermissionStatus );
	void			SetClientsConnectionAuthorisation( int PermissionStatus );
	void			SetClientsConnectionNickCmd( int PermissionStatus );
	void			SetClientsConnectionUserCmd( int PermissionStatus );

	/*-------------------------------------------*/
	/*--------------All the Getters--------------*/

	int				GetChanLim( void );
	int				GetFirstMOTD( void );
	int				GetClientsFd( void );
	int				GetClientsConnectionStatus( void );
	int				GetClientsConnectionNickCmd( void );
	int				GetClientsConnectionUserCmd( void );
	int				GetClientsConnectionPermission( void );
	int				GetClientsConnectionAuthorisation( void );

	time_t			GetClientsLastPing( void );

	std::string		GetServerName( void );
	std::string		GetClientsMode( void );
	std::string		GetClientsUnused( void );
	std::string 	GetClientsMessage( void );
	std::string		GetClientsBuffer( void );
	std::string		GetClientsRealname( void );
	std::string		GetClientsNickname( void );
	std::string 	GetClientsHostname( void );
	std::string		GetClientsUsername( void );
	std::string 	GetClientsHostAdress( void );

	/*-------------------------------------------*/
	/*===========================================*/

	int				DeleteClientsMode( char DeleteMode );
	int				AddClientsMode( char AddMode );

private:
    Clients(void);
	Clients( const Clients & copy );
	Clients & operator=( const Clients & client );

	int				_ChanLim;
	int 			_ClientFd;
	int				_FirstMOTD;
	int				_ConnectionStatus;

	time_t			_last_com;
	time_t			_LastPing;
	sockaddr_in 	_New_Address;

	std::string 	_Mode; 
	std::string 	_Buffer;
	std::string		_Unused;
	std::string		_Username;
	std::string 	_Realname;
	std::string		_ClientMessage;

	bool			_HasTheClientsBeenAccepted;   
	bool			_HasTheClientsBeenAuthorized; 
	bool			_HasTheClientsNickBeenChosen; 
	bool			_HasTheClientsUserBeenChosen; 
	
};

# endif