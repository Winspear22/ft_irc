#ifndef CLIENTS_HPP
# define CLIENTS_HPP

# include "irc.hpp"
class Clients
{
public:
	Clients( int ClientFd, struct sockaddr_in _New_Address, std::string ServerName );
	Clients( const Clients & copy );
	Clients & operator=( const Clients & client );
	~Clients( void );

	int		GetClientFd( void );

	void	SetClientFd( int ClientFd );

private:
    Clients(/* ARG*/);
	int 		_ClientFd;
	sockaddr_in _New_Address;
	std::string _ServerName;

};
# endif