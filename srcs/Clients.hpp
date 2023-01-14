#ifndef CLIENTS_HPP
# define CLIENTS_HPP

# include "irc.hpp"
class MyMsg;

class Clients
{
public:
	Clients( int ClientFd, struct sockaddr_in _New_Address, std::string ServerName );
	Clients( const Clients & copy );
	Clients & operator=( const Clients & client );
	~Clients( void );

	int			GetClientsFd( void );
	std::string	GetServerName( void );
	void		SetServerName( std::string ServerName);
	std::string	GetClientsUsername( void );
	void		SetClientsUsername( std::string username );
	std::string	GetClientsMode( void );
	void		SetClientsMode( std::string Mode );
	std::string	GetClientsUnused( void );
	void		SetClientsUnused( std::string Unused );
	std::string	GetClientsRealname( void );
	void		SetClientsRealname( std::string realname );
	std::string	GetClientsNickname( void );
	void		SetClientsNickname( std::string nickname );
	std::string GetClientsHostname( void );
	void		SetClientsHostname( std::string hostname );
	std::string GetClientsHostAdress( void );
	void		SetClientsHostAdress( std::string hostAdress );
	int			GetClientsConnectionPermission( void );
	void		SetClientsConnectionPermission( int PermissionStatus );
	int			GetClientsConnectionAuthorisation( void );
	void		SetClientsConnectionAuthorisation( int PermissionStatus );
	int			GetClientsConnectionNickCmd( void );
	void		SetClientsConnectionNickCmd( int PermissionStatus );
	int			GetClientsConnectionUserCmd( void );
	void		SetClientsConnectionUserCmd( int PermissionStatus );
	std::string	GetClientsBuffer( void );
	void		SetClientsBuffer( std::string );
	int			GetClientsConnectionStatus( void );
	void		SetClientsConnectionStatus( int status );
	time_t		GetClientsLastPing( void );
	void		SetClientsLastPing( time_t ping);



	std::string GetClientsMessage( void );
	void		SetClientsMessage( std::string msg_recv );
	int			DeleteClientsMode( char DeleteMode );
	int			AddClientsMode( char AddMode );

/*A SUPPRIMER A LA FIN DU TEST*/

void	resetTime(void);



private:
    Clients(/* ARG*/);
	int 		_ClientFd;
	sockaddr_in _New_Address;
	std::string _ServerName;
	std::string _Buffer;

	/*Clients Identity*/
	std::string	_Username;
	std::string _Mode; // Sert à la commande MODE, il y'a 2 modes --> à voir plus tard lors de la prgmtion de MODE
	std::string	_Unused; // ce paramètre n'est pas utilisé par les clients selon RFC
	std::string _Realname;
	std::string	_Nickname;
	std::string _Hostname;
	std::string	_HostAdress;
	time_t		_LastPing;

	/*On utilise les deux derniers booléens car Nick et User peuvent être faits dans des ordres différents*/
	/*Autrement : Authorisation + NICK + USER = Permission */
	bool		_HasTheClientsBeenAccepted;   // Authorisation gbl pour Nick/User/Pass pour valider entièrement un user
	bool		_HasTheClientsBeenAuthorized; // Authorisation pour le PWD
	bool		_HasTheClientsNickBeenChosen; // Authorisation pour le Nick - NICK a bien fonctionné
	bool		_HasTheClientsUserBeenChosen; // Authorisation pour le User - USER a bien fonctionné
	int			_ConnectionStatus;
	/*Clients Message*/
	std::string	_ClientMessage;
	
	/*A supprimer à la fin du test*/
	time_t	_last_com;


};
# endif