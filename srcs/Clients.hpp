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
	std::string	GetClientsRealname( void );
	std::string	GetClientsNickname( void );

	std::string GetClientsMessage( void );
	void		SetClientsMessage( std::string msg_recv );
private:
    Clients(/* ARG*/);
	int 		_ClientFd;
	sockaddr_in _New_Address;
	std::string _ServerName;

	/*Clients Identity*/
	std::string	_Username;
	std::string _Realname;
	std::string	_Nickname;

	/*Clients Message*/
	std::string	_ClientMessage;

};
# endif