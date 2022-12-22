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
	std::string	GetClientsHostname( void );
	void		SetClientsHostname( std::string hostname );
	std::string	GetClientsRealname( void );
	void		SetClientsRealname( std::string realname );
	std::string	GetClientsNickname( void );
	void		SetClientsNickname( std::string nickname );
	int			GetClientsConnectionPermission( void );
	void		SetClientsConnectionPermission( int PermissionStatus );
	int			GetClientsConnectionAuthorisation( void );
	void		SetClientsConnectionAuthorisation( int PermissionStatus );


	std::string GetClientsMessage( void );
	void		SetClientsMessage( std::string msg_recv );
private:
    Clients(/* ARG*/);
	int 		_ClientFd;
	sockaddr_in _New_Address;
	std::string _ServerName;

	/*Clients Identity*/
	std::string	_Username;
	std::string _Hostname;
	std::string _Realname;
	std::string	_Nickname;
	bool		_HasTheClientsBeenAccepted;
	bool		_HasTheClientsBeenAuthorized;

	/*Clients Message*/
	std::string	_ClientMessage;

};
# endif