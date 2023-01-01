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
	int			GetClientsConnectionPermission( void );
	void		SetClientsConnectionPermission( int PermissionStatus );
	int			GetClientsConnectionAuthorisation( void );
	void		SetClientsConnectionAuthorisation( int PermissionStatus );
	int			GetClientsConnectionNickCmd( void );
	void		SetClientsConnectionNickCmd( int PermissionStatus );
	int			GetClientsConnectionUserCmd( void );
	void		SetClientsConnectionUserCmd( int PermissionStatus );




	std::string GetClientsMessage( void );
	void		SetClientsMessage( std::string msg_recv );
private:
    Clients(/* ARG*/);
	int 		_ClientFd;
	sockaddr_in _New_Address;
	std::string _ServerName;

	/*Clients Identity*/
	std::string	_Username;
	std::string _Mode; // Sert à la commande MODE, il y'a 2 modes --> à voir plus tard lors de la prgmtion de MODE
	std::string	_Unused; // ce paramètre n'est pas utilisé par les clients selon RFC
	std::string _Realname;
	std::string	_Nickname;

	/*On utilise les deux derniers booléens car Nick et User peuvent être faits dans des ordres différents*/
	/*Autrement : Authorisation + NICK + USER = Permission */
	bool		_HasTheClientsBeenAccepted; // Authorisation pour le PWD
	bool		_HasTheClientsBeenAuthorized; // Authorisation gbl pour Nick/User/Pass pour valider entièrement un user
	bool		_HasTheClientsNickBeenChosen; // Authorisation pour le Nick - NICK a bien fonctionné
	bool		_HasTheClientsUserBeenChosen; // Authorisation pour le User - USER a bien fonctionné

	/*Clients Message*/
	std::string	_ClientMessage;

};
# endif