# include "Clients.hpp"

Clients::Clients( void )
{
	//std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}


Clients::Clients( int ClientFd, struct sockaddr_in New_Address, std::string ServerName )
{
	this->_ClientFd = ClientFd;
	this->_New_Address = New_Address;
	this->_ServerName = ServerName;
	this->_HasTheClientsBeenAccepted = NO;
	this->_HasTheClientsBeenAuthorized = NO;
	this->_HasTheClientsUserBeenChosen = NO;
	this->_HasTheClientsNickBeenChosen = NO;
	this->_ConnectionStatus = YES;
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
