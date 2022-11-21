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

void			MyServer::AcceptClientsConnections( void )
{
	//int ret;
	int sockadress_len;

	sockadress_len = sizeof(this->_sockadress);
	//std::cout << BLUE << "je suis avant le accept." << std::endl;
	this->_acceptsocket = accept(this->_socketfd, (struct sockaddr *)&this->_sockadress, (socklen_t * )&sockadress_len);
	//std::cout << BLUE << "je suis apres le accept." << std::endl;
	//if (this->_acceptsocket == ERROR_SERVER)
		//SelectClients();
	//	loop_errors_handlers_msg(ERROR_ACCEPT);
	if (this->_acceptsocket >= 0)
	{
		std::cout << GREEN << "Accept success !" << NORMAL << std::endl;
		//SelectClients();
	}
}

int				MyServer::AcceptNewClientsConnections( void )
{
	int new_client_fd;
    new_client_fd = accept(this->_socketfd, NULL, NULL);
	if (this->_acceptsocket == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_ACCEPT);
	return (new_client_fd);
}

void			MyServer::SelectClients( void )
{
	fd_set	ready_fds; //mes fds etant prets a transmettre des donnes
	fd_set	readfds; // mes sets de fds pouvant lire
	//fd_set	writefds; // mes sets de fds pouvant ecrire
	int		maximum_fds; // mon nombre max de fds peut etre remplace par FD_ISSET
	int		ret_select; //return de select pour les erreurs
	int		fds_list; // sert a loop pour trouver lequel des fds a des donnees pour moi
	int		new_client_fd; // nouvel utilisateur se connecte et cree une nouvelle socket IL FAUT CREER UNE FONCTION SPECIALE POUR
	
	fds_list = -1;
	FD_ZERO(&ready_fds);
	FD_SET(this->_socketfd, &ready_fds);
	readfds = ready_fds; // je sais plus pk on fait ca 
	maximum_fds = this->_socketfd;
	std::cout << PURPLE << "je suis avant le select." << std::endl;
	ret_select = select(maximum_fds + 1, &readfds, NULL, NULL, NULL);
	std::cout << PURPLE << "je suis apres le select." << std::endl;
	if (ret_select == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_SELECT);
	while (++fds_list <= maximum_fds) //on doit checker ce qui se passe sur tous les fds un par un
	{
		if (fds_list == this->_socketfd) // C'est un client qui a ete trouve
		{
			std::cout << GREEN << "New connection incoming !" << std::endl;
			new_client_fd = AcceptNewClientsConnections();
			std::cout << RED << "new client fds " << new_client_fd << std::endl;
			FD_SET(new_client_fd, &ready_fds);
			if (new_client_fd > maximum_fds)
                     maximum_fds = new_client_fd;
			std::cout << RED << "Max fds " << maximum_fds << std::endl;
		}
		else
		{
			std::cout << GREEN << "New message incoming ! Message from fd : " << WHITE << fds_list << NORMAL << std::endl;
			this->RecvMsg( fds_list );
		}
	}
	fds_list = 0;
}

void			MyServer::RecvMsg( int current_fd )
{
	char buff[999999];
	int ret_recv;
	int	ret_send;

	ret_recv = recv(current_fd, buff, strlen(buff), 0);
	if (ret_recv == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_RECV);
	std::cout << "BUFF == " << buff << std::endl;
	if (ret_recv == 0) // client disconnected
		std::cout << RED << "Error. The client has disconnected." << std::endl;
	ret_send = send(current_fd, buff, strlen(buff), 0);
	if (ret_send == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_SEND);


	
}

/*void			MyServer::RecvAndSend( void )
{
	char buff[999999];
	char msg_sent[42] = "\033[1;35mSalut les amis. Ca marche !\033[0m";
	int ret_send;
	int ret_recv;*/
	/*ON NE MET PAS DE PROTECTION CAR TANT QUE RIEN N'EST SEND OU RECV, IL RENVOIT -1 EN PERMANENCE*/
	/*ret_recv = recv(this->_acceptsocket, buff, strlen(buff), 0);
	if (ret_recv == ERROR_SERVER && this->_one_client_has_connected == 1)
		loop_errors_handlers_msg(ERROR_RECV);
	ret_send = send(this->_acceptsocket, msg_sent, strlen(msg_sent), 0);
	if (ret_send == ERROR_SERVER && this->_one_client_has_connected == 1) // && this->_one_client_has_connected == 1 pour retirer le spam du message d'erreur pdt le wait du client
		loop_errors_handlers_msg(ERROR_SEND);
}*/