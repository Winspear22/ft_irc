# include "Clients.hpp"
# include "MyMsg.hpp"

Clients::Clients( void )
{
	//std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}


Clients::Clients( int ClientFd, struct sockaddr_in New_Address, std::string ServerName )
{
	char hostname[NI_MAXHOST];
	int ret;

	this->_ClientFd = ClientFd;
	//a supprimer à la fin du test :
	//this->_last_com = time(0);
	//----------
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
	ret = getnameinfo(((struct sockaddr *)&New_Address), sizeof(New_Address), hostname, NI_MAXHOST, NULL, 0, 0);
	if (ret == SUCCESS)
		this->_Hostname = hostname;
	else
		loop_errors_handlers_msg(ERROR_USER_DISCONNECTED);
	//std::cout << GREEN << "Clients Constructor called." << NORMAL << std::endl;
	return ;
}

Clients::Clients( const Clients & copy )
{
	//std::cout << "\033[0;33mClients Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

Clients & Clients::operator=( Clients const & rhs )
{
	//std::cout << "\033[0;34mClients Copy assignment operator called." << NORMAL << std::endl;
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
	//std::cout << RED << "Clients Destructor called." << NORMAL << std::endl;
	return ;
}

int		Clients::GetClientsFd( void )
{
	return (this->_ClientFd);
}

std::string Clients::GetClientsUsername( void )
{
	return (this->_Username);
}

void		Clients::SetClientsUsername( std::string username )
{
	this->_Username = username;
}

std::string	Clients::GetClientsMode( void )
{
	return (this->_Mode);
}

void		Clients::SetClientsMode( std::string mode )
{
	this->_Mode = mode;
}

std::string	Clients::GetClientsUnused( void )
{
	return (this->_Unused);
}

void		Clients::SetClientsUnused( std::string Unused )
{
	this->_Unused = Unused;
}

std::string Clients::GetClientsRealname( void )
{
	return (this->_Realname);
}

void		Clients::SetClientsRealname( std::string Realname )
{
	this->_Realname = Realname;
}


std::string Clients::GetClientsNickname( void )
{
	return (this->_Nickname);
}

void		Clients::SetClientsNickname( std::string Nickname )
{
	this->_Nickname = Nickname;
}

std::string Clients::GetClientsHostname( void )
{
	return (this->_Hostname);
}

void		Clients::SetClientsHostname( std::string Hostname )
{
	this->_Hostname = Hostname;
}

std::string Clients::GetClientsHostAdress( void )
{
	return (this->_HostAdress);
}

void		Clients::SetClientsHostAdress( std::string hostAdress )
{
	this->_HostAdress = hostAdress;
}

std::string Clients::GetClientsMessage( void )
{
	return (this->_ClientMessage);
}

void		Clients::SetClientsMessage( std::string msg_recv )
{
	this->_ClientMessage = msg_recv;
}

int			Clients::GetClientsConnectionPermission( void )
{
	return (this->_HasTheClientsBeenAccepted);
}

void		Clients::SetClientsConnectionPermission( int PermissionStatus )
{
	this->_HasTheClientsBeenAccepted = PermissionStatus;
}

int			Clients::GetClientsConnectionAuthorisation( void )
{
	return (this->_HasTheClientsBeenAuthorized);
}

void		Clients::SetClientsConnectionAuthorisation( int PermissionStatus )
{
	this->_HasTheClientsBeenAuthorized = PermissionStatus;
}

int			Clients::GetClientsConnectionNickCmd( void )
{
	return (this->_HasTheClientsNickBeenChosen);
}

void		Clients::SetClientsConnectionNickCmd( int PermissionStatus )
{
	this->_HasTheClientsNickBeenChosen = PermissionStatus;
}

int			Clients::GetClientsConnectionUserCmd( void )
{
	return (this->_HasTheClientsUserBeenChosen);
}

void		Clients::SetClientsConnectionUserCmd( int PermissionStatus )
{
	this->_HasTheClientsUserBeenChosen = PermissionStatus;
}
/*Ces deux fonctions servent au cas où le messge reçu serait incomplet avec recv*/
std::string	Clients::GetClientsBuffer( void )
{
	return (this->_Buffer);
}
void		Clients::SetClientsBuffer( std::string msg )
{
	this->_Buffer = msg;
}

int			Clients::GetClientsConnectionStatus( void )
{
	return (this->_ConnectionStatus);
}
void		Clients::SetClientsConnectionStatus( int status )
{
	this->_ConnectionStatus = status;
}

std::string	Clients::GetServerName( void )
{
	return (this->_ServerName);
}

void		Clients::SetServerName( std::string ServerName )
{
	this->_ServerName = ServerName;
}

time_t		Clients::GetClientsLastPing( void )
{
	return (time(0) - this->_LastPing);
}
void		Clients::SetClientsLastPing( time_t ping )
{
	this->_LastPing = ping;
}

int			Clients::DeleteClientsMode( char DeleteMode )
{

	if (this->_Mode.find(DeleteMode) == std::string::npos)
		return (FAILURE);
	else
		this->_Mode.erase(this->_Mode.find(DeleteMode), 1);
	return (SUCCESS);

}

int			Clients::AddClientsMode( char AddMode )
{
	if (this->_Mode.find(AddMode) != std::string::npos)
	{
		return (FAILURE);
	}
	else
	{
		this->_Mode.push_back(AddMode);
	}
	return (SUCCESS);
}

/*a supprimer a la fin du test*/

void	Clients::resetTime(void)
{
	this->_last_com = time(0);
}

int			Clients::GetFirstMOTD( void )
{
	return (this->_FirstMOTD);
}

void		Clients::SetFirstMOTD( int motd )
{
	this->_FirstMOTD = motd;
}