# include "Clients.hpp"
# include "MyMsg.hpp"

Clients::Clients( void )
{
	return ;
}

Clients::Clients( int ClientFd, struct sockaddr_in New_Address, std::string ServerName )
{
	char hostname[NI_MAXHOST];
	int ret;

	this->_ClientFd = ClientFd;
	this->_New_Address = New_Address;
	this->_ServerName = ServerName;
	this->_HasTheClientsBeenAccepted = NO;
	this->_HasTheClientsBeenAuthorized = NO;
	this->_HasTheClientsUserBeenChosen = NO;
	this->_HasTheClientsNickBeenChosen = NO;
	this->_ConnectionStatus = YES;
	this->_HostAdress = inet_ntoa(New_Address.sin_addr);
	this->_LastPing = time(0);
	this->_FirstMOTD = 0;
	this->_ChanLim = 0;
	ret = getnameinfo(((struct sockaddr *)&New_Address), sizeof(New_Address), hostname, NI_MAXHOST, NULL, 0, 0);
	if (ret == SUCCESS)
		this->_Hostname = hostname;
	else
		loop_errors_handlers_msg(ERROR_USER_DISCONNECTED);
	return ;
}

Clients::Clients( const Clients & copy )
{
	*this = copy;
    return ;
}

Clients & Clients::operator=( Clients const & rhs )
{
	if ( this != &rhs )
    {
	    this->_ClientFd = rhs._ClientFd;
	    this->_New_Address = rhs._New_Address;
	    this->_ServerName = rhs._ServerName;
    }
	return (*this);
}


Clients::~Clients( void )
{
	return ;
}

	/*--------------All the Setters--------------*/

void					Clients::SetClientsMode( std::string mode )
{
	this->_Mode = mode;
}

void					Clients::SetClientsUsername( std::string username )
{
	this->_Username = username;
}

void					Clients::SetClientsRealname( std::string Realname )
{
	this->_Realname = Realname;
}

void					Clients::SetClientsUnused( std::string Unused )
{
	this->_Unused = Unused;
}

void					Clients::SetClientsNickname( std::string Nickname )
{
	this->_Nickname = Nickname;
}

void					Clients::SetClientsHostname( std::string Hostname )
{
	this->_Hostname = Hostname;
}

void					Clients::SetClientsHostAdress( std::string hostAdress )
{
	this->_HostAdress = hostAdress;
}

void					Clients::SetClientsMessage( std::string msg_recv )
{
	this->_ClientMessage = msg_recv;
}

void					Clients::SetClientsConnectionPermission( int PermissionStatus )
{
	this->_HasTheClientsBeenAccepted = PermissionStatus;
}

void					Clients::SetClientsConnectionAuthorisation( int PermissionStatus )
{
	this->_HasTheClientsBeenAuthorized = PermissionStatus;
}

void					Clients::SetClientsConnectionNickCmd( int PermissionStatus )
{
	this->_HasTheClientsNickBeenChosen = PermissionStatus;
}

void					Clients::SetClientsConnectionUserCmd( int PermissionStatus )
{
	this->_HasTheClientsUserBeenChosen = PermissionStatus;
}

void					Clients::SetClientsBuffer( std::string msg )
{
	this->_Buffer = msg;
}

void					Clients::SetClientsConnectionStatus( int status )
{
	this->_ConnectionStatus = status;
}

void					Clients::SetServerName( std::string ServerName )
{
	this->_ServerName = ServerName;
}

void					Clients::SetClientsLastPing( time_t ping )
{
	this->_LastPing = ping;
}

void					Clients::SetChanLim( int ChanLim )
{
	this->_ChanLim = ChanLim;
}


	/*--------------All the Getters--------------*/


int						Clients::GetClientsFd( void )
{
	return (this->_ClientFd);
}

std::string 			Clients::GetClientsUsername( void )
{
	return (this->_Username);
}

std::string				Clients::GetClientsMode( void )
{
	return (this->_Mode);
}

std::string				Clients::GetClientsUnused( void )
{
	return (this->_Unused);
}

std::string 			Clients::GetClientsRealname( void )
{
	return (this->_Realname);
}

std::string 			Clients::GetClientsNickname( void )
{
	return (this->_Nickname);
}

std::string 			Clients::GetClientsHostname( void )
{
	return (this->_Hostname);
}

std::string 			Clients::GetClientsHostAdress( void )
{
	return (this->_HostAdress);
}

std::string 			Clients::GetClientsMessage( void )
{
	return (this->_ClientMessage);
}

int						Clients::GetClientsConnectionPermission( void )
{
	return (this->_HasTheClientsBeenAccepted);
}

int						Clients::GetClientsConnectionAuthorisation( void )
{
	return (this->_HasTheClientsBeenAuthorized);
}

int						Clients::GetClientsConnectionNickCmd( void )
{
	return (this->_HasTheClientsNickBeenChosen);
}

int						Clients::GetClientsConnectionUserCmd( void )
{
	return (this->_HasTheClientsUserBeenChosen);
}

std::string				Clients::GetClientsBuffer( void )
{
	return (this->_Buffer);
}

int						Clients::GetClientsConnectionStatus( void )
{
	return (this->_ConnectionStatus);
}

std::string				Clients::GetServerName( void )
{
	return (this->_ServerName);
}

time_t					Clients::GetClientsLastPing( void )
{
	return (time(0) - this->_LastPing);
}

int						Clients::GetChanLim( void )
{
	return (this->_ChanLim);
}

	/*-------------------------------------------*/


int						Clients::DeleteClientsMode( char DeleteMode )
{

	if (this->_Mode.find(DeleteMode) == std::string::npos)
		return (FAILURE);
	else
		this->_Mode.erase(this->_Mode.find(DeleteMode), 1);
	return (SUCCESS);

}

int						Clients::AddClientsMode( char AddMode )
{
	if (this->_Mode.find(AddMode) != std::string::npos)
		return (FAILURE);
	else
		this->_Mode.push_back(AddMode);

	return (SUCCESS);
}

void					Clients::resetTime(void)
{
	this->_last_com = time(0);
}

int						Clients::GetFirstMOTD( void )
{
	return (this->_FirstMOTD);
}

void					Clients::SetFirstMOTD( int motd )
{
	this->_FirstMOTD = motd;
}