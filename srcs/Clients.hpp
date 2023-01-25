#ifndef CLIENTS_HPP
# define CLIENTS_HPP

# include "irc.hpp"
class MyMsg;

class Clients
{
public:
	Clients( int ClientFd, struct sockaddr_in _New_Address, std::string ServerName );
	~Clients( void );
	/*===========================================*/
	/*             GETTERS AND SETTERS           */
	/*--------------All the Setters--------------*/
	void			SetFirstMOTD(int motd);
	void			SetChanLim(int ChanLim);
	void			SetClientsBuffer(std::string);
	void			SetClientsLastPing(time_t ping);
	void			SetClientsMode(std::string Mode);
    void			SetClientsUnused(std::string Unused);
	void			SetServerName(std::string ServerName);
	void			SetClientsConnectionStatus(int status);
	void			SetClientsMessage(std::string msg_recv);
	void			SetClientsUsername(std::string username);
	void			SetClientsRealname(std::string realname);
	void			SetClientsNickname(std::string nickname);
	void			SetClientsHostname(std::string hostname);
	void			SetClientsHostAdress(std::string hostAdress);
	void			SetClientsConnectionUserCmd(int PermissionStatus);
	void			SetClientsConnectionNickCmd(int PermissionStatus);
	void			SetClientsConnectionPermission(int PermissionStatus);
	void			SetClientsConnectionAuthorisation(int PermissionStatus);


	/*-------------------------------------------*/
	/*--------------All the Getters--------------*/
	int				GetChanLim(void);
	int				GetClientsFd(void);
	int				GetFirstMOTD(void);
	int				GetClientsConnectionStatus(void);
	int				GetClientsConnectionNickCmd(void);
	int				GetClientsConnectionUserCmd(void);
	int				GetClientsConnectionPermission(void);
	int				GetClientsConnectionAuthorisation(void);

	time_t			GetClientsLastPing(void);

	std::string		GetServerName(void);
	std::string		GetClientsUsername(void);
	std::string		GetClientsMode(void);
	std::string		GetClientsUnused(void);
	std::string		GetClientsBuffer(void);
	std::string		GetClientsRealname(void);
	std::string 	GetClientsNickname(void);
	std::string 	GetClientsHostname(void);
	std::string 	GetClientsHostAdress(void);
	std::string 	GetClientsMessage( void );
	/*-------------------------------------------*/
	/*===========================================*/

	int				DeleteClientsMode(char DeleteMode);
	int				AddClientsMode(char AddMode);

	void			resetTime(void);
	
	std::string		_Nickname;
	std::string 	_Hostname;
	std::string		_HostAdress;
	std::string 	_ServerName;

private:
    Clients();
	Clients( const Clients & copy );
	Clients & operator=( const Clients & client );

	sockaddr_in 	_New_Address;

	int				_ChanLim;
	int 			_ClientFd;
	int				_FirstMOTD;
	int				_ConnectionStatus;

	bool			_HasTheClientsBeenAccepted;
	bool			_HasTheClientsBeenAuthorized;
	bool			_HasTheClientsNickBeenChosen; 
	bool			_HasTheClientsUserBeenChosen; 

	time_t			_last_com;
	time_t			_LastPing;

	std::string 	_Mode;
	std::string		_Username;
	std::string		_Unused;
	std::string 	_Buffer;
	std::string 	_Realname;
	std::string		_ClientMessage;
};
# endif