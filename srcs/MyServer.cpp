# include "MyServer.hpp"

MyServer::MyServer( void )
{
	std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}


MyServer::MyServer( int port, std::string password ): _port(port), _password(password), _server_status(SERVER_ON)
{
	std::cout << GREEN << "MyServer Constructor called." << NORMAL << std::endl;
	return ;
}

MyServer::MyServer( const MyServer & copy )
{
	std::cout << "\033[0;33mMyServer Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

MyServer::~MyServer( void )
{
	std::cout << RED << "MyServer Destructor called." << NORMAL << std::endl;
	return ;
}

MyServer & MyServer::operator=( MyServer const & rhs )
{
	std::cout << "\033[0;34mMyServer Copy assignment operator called." << NORMAL << std::endl;
	if ( this != &rhs )
    {
        this->_password = rhs._password;
		this->_port = rhs._port;
		this->_server_status = rhs._server_status;
    }
	return (*this);
}

void        MyServer::SetPort( char *str )
{
	this->_port = atoi(str);
}

void        MyServer::SetPassword( char *str )
{
	this->_password = str;
}

void        MyServer::SetServerStatus( int ServerStatus )
{
	this->_server_status = ServerStatus;
}

/*								ALL THE GETTERS								*/
int     MyServer::GetPort( void )
{
    return (this->_port);
}

int		MyServer::GetServerStatus( void )
{
	return (this->_server_status);
}

std::string     MyServer::GetPassword( void )
{
    return (this->_password);
}

int			MyServer::CreateSocketFd( void )
{
	this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketfd == ERROR_SERVER)
		return (ERROR_SOCKET_CREATION);
	return (SUCCESS);
}

int			MyServer::SetSocketOptions( void )
{
	int			ret;
	const int 	optname = 1; //l'option SO_REUSEADDR REQUIERT UN BOOLEEAN POUR FONCTIONNER, ON SET ALORS CET ARGUMENT A 1, DONC A VRAI, POUR QUE LA FCT MARCHE.

	ret = setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &optname,  sizeof(int));
	if (ret == ERROR_SERVER)
		return (ERROR_SOCKET_OPTIONS);
	return (SUCCESS);
}


int			MyServer::BindSocketFd( void )
{
	int ret;

	this->_sockadress.sin_port = htons(this->_port);
	this->_sockadress.sin_family = AF_INET; 
	this->_sockadress.sin_addr.s_addr = INADDR_ANY;
	/*IL FAUT PROTEGER CETTE VARIABLE CAR INET_ADDR RENVOIT -1 EN CAS D'ECHEC ET -1 = 255.255.255.255 IP, IP UNIVERSELLE*/
	//if (this->_sockadress.sin_addr.s_addr < 0)
	//	return (ERROR_SOCKET_BINDING);
	/*if ((this->_sockadress.sin_addr.s_addr = INADDR_ANY) < 0)
		return (ERROR_SOCKET_BINDING);*/
	ret = bind(this->_socketfd, (struct sockaddr *)&this->_sockadress, sizeof(this->_sockadress));
	if (ret == ERROR_SERVER)
		return (ERROR_SOCKET_BINDING);
	std::cout << GREEN << "BIND(); WORKED." << NORMAL << std::endl;
	return (SUCCESS);
}

int			MyServer::ListenToSockedFd( void )
{
	int ret;

	ret = listen(this->_socketfd, 10);
	if (ret == ERROR_SERVER)
		return (ERROR_LISTENING);
	std::cout << GREEN << "listen(); works !" << std::endl;
	return (SUCCESS);
}

int			MyServer::SetSocketFdToNonBlocking( void )
{
	int ret;
	/*COMPRENDRE LE FLAG F_SETFL*/
	/*Si on retire cette partie, la connexion devient bloquante*/
	ret = fcntl(this->_socketfd, F_SETFL, O_NONBLOCK);
	if (ret == ERROR_SERVER)
		return (ERROR_NONBLOCKING);
	std::cout << GREEN << "fcntl(); works !" << std::endl;
	return (SUCCESS);
}

void			MyServer::set_pollfd( std::vector<struct pollfd> &fds )
{
	fds.clear();
	fds.push_back(pollfd());
	fds.back().fd = this->_socketfd;
	fds.back().events = POLLIN;
	fds.back().revents = 0;
}

void	MyServer::serv_accept(std::vector<pollfd> &fds)
{
	int				new_client_fd;
	struct sockaddr	cs;
	socklen_t		cs_len = sizeof(cs);
	new_client_fd = accept(this->_socketfd, &cs, &cs_len);
	if (new_client_fd == -1)
		return ;
	Clients *new_client = new Clients(new_client_fd, *reinterpret_cast<struct sockaddr_in*>(&cs), "MyServerName");
	this->_clients_list.insert(std::make_pair(new_client, new_client_fd));
/*	if (serv->getUsers().size() > serv->getConfig()->getMaxUsers())
	{
		new_client->disconnect();
		fds.push_back(pollfd());
		fds.back().fd = new_client_fd;
		fds.back().events = 0;
		fds.back().revents = 0;
		return ;
	}*/

	fds.push_back(pollfd());
	fds.back().fd = new_client_fd;
	fds.back().events = POLLIN;
	fds.back().revents = POLLIN;

	std::cout << "New Client on socket #" << new_client_fd << "." << std::endl;
}


void			MyServer::AcceptClientsConnections( void )
{
	std::vector<struct pollfd> fds;
	int timeout;
	unsigned int i;
	int ret_poll;

	/*CREER UNE FONCTION QUI INITIALISE MES FDS*/
	timeout = (3 * 3 * 1000);
	i = 0;
	set_pollfd(fds);
	ret_poll = poll(fds.data(), fds.size(), timeout);
	if (ret_poll == TIMEOUT) // REGLER LE RETURN ;
		return ;
	if (ret_poll == -1) // REGLER LE RETURN ;
		return ;
	while (i < fds.size())
	{
		if (fds[i].revents == POLLIN)
		{
			if (i == 0)
			{
				std::cout << "SERVER ACCEPT" << std::endl;
				serv_accept(fds);		
			}
			if (i != 0)
					std::cout << "SERVER RECEIVE" << std::endl;
					/*SERVER RECEIVE*/
		}
		if (fds[i].revents == POLLHUP || fds[i].revents == POLLERR || fds[i].revents == POLLNVAL)
		{	
				std::cout << "Invalid event on socket #" << fds[i].fd << "." << std::endl;
				//serv->getUser(fds[n].fd)->disconnect();
		}
		i++;
	}
		/*RM DECO
		RM CHANS */
}

void			MyServer::SelectClients( void )
{
	fd_set	fds;
	fd_set	readfds;
	//fd_set	writefds;
	int		maximum_clients;
	int		ret_select;

	FD_ZERO(&fds);
	FD_SET(this->_socketfd, &fds);
	readfds = fds;
	maximum_clients = this->_socketfd;
	std::cout << PURPLE << "je suis avant le select." << std::endl;
	ret_select = select(maximum_clients + 1, &readfds, NULL, NULL, NULL);
	std::cout << PURPLE << "je suis apres le select." << std::endl;
	if (ret_select == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_SELECT);
}

void			MyServer::RecvAndSend( void )
{
	char buff[999999];
	char msg_sent[42] = "\033[1;35mSalut les amis. Ca marche !\033[0m";
	int ret_send;
	int ret_recv;
	/*ON NE MET PAS DE PROTECTION CAR TANT QUE RIEN N'EST SEND OU RECV, IL RENVOIT -1 EN PERMANENCE*/
	ret_recv = recv(this->_acceptsocket, buff, strlen(buff), 0);
	if (ret_recv == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_RECV);
	ret_send = send(this->_acceptsocket, msg_sent, strlen(msg_sent), 0);
	if (ret_send == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_SEND);
}