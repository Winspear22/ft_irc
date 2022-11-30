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
	std::map<Clients*, int>::iterator it;
	
	it = this->_clients_list.begin();	
	while (it != this->_clients_list.end())
	{
		delete it->first;
		std::cout << YELLOW << "Deleting client n° : " << WHITE << it->second << NORMAL << std::endl;
		it++;
	}
	this->_clients_list.clear();
	std::cout << CYAN << "All Clients were freed. No Leaks. :)" << NORMAL << std::endl;
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

void		MyServer::InitVariables( void )
{
	this->_new_fd_nb = 0;
	this->_nb_of_clients = 0;
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


	#include <stdio.h>
	#include <time.h>

const char *dot_str[] = {".", ".", ".", "\b\b\b   \b\b\b"};
#define countof(x) (sizeof(x)/sizeof((x)[0]))

static int next_state = 0;
void update_progress(void) {
    fputs(dot_str[next_state], stdout);
    next_state = (next_state + 1) % countof(dot_str);
    fflush(stdout);
}

static time_t last_time = 0;
void update_progress_if_time(void) {
    time_t now = time(NULL);
    if(now > last_time) {
        update_progress();
        last_time = now;
    }
}

void start_progress(const char *loading) {
    fputs(loading, stdout);
    next_state = 0;
    last_time = 0;
    fflush(stdout);
}


int			MyServer::SelectClients( void )
{
	fd_set	ready_fds; //mes fds etant prets a transmettre des donnes
	fd_set	readfds; // mes sets de fds pouvant lire
	int		maximum_fds; // mon nombre max de fds peut etre remplace par FD_ISSET
	int		ret_select; //return de select pour les erreurs
	int		fds_list; // sert a loop pour trouver lequel des fds a des donnees pour moi
	struct timeval		timeout;

	timeout.tv_sec = 120;
	fds_list = -1;
	FD_ZERO(&ready_fds);
	FD_SET(this->_socketfd, &ready_fds);
	readfds = ready_fds; // je sais plus pk on fait ca 
	maximum_fds = this->_socketfd;
	ret_select = select(maximum_fds + 1, &readfds, NULL, NULL, &timeout);
	if (ret_select == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_SELECT);
	if (ret_select == TIMEOUT)
		errors_handlers_msg(TIMEOUT);
	while (++fds_list <= maximum_fds) //on doit checker ce qui se passe sur tous les fds un par un
	{
		update_progress_if_time();
		if (fds_list == this->_socketfd) // C'est un client qui a ete trouve
		{
			this->CreateClients();
			FD_SET(this->_new_fd_nb, &ready_fds);
			if (this->_new_fd_nb > maximum_fds)
                     maximum_fds = this->_new_fd_nb;
			std::cout << CYAN << "Client currently connected : " << this->_nb_of_clients << std::endl;
		}
		else
			RecvClientsMsg(this->_new_fd_nb);
	}
	return (SUCCESS);
}


void			MyServer::CreateClients( void )
{
	int				client_created_fd;
	Clients			*client_created;
	struct sockaddr	client_addr;
	socklen_t		sizeofsockaddr;
	
	sizeofsockaddr = sizeof(client_addr);
	client_created_fd = accept(this->_socketfd, &client_addr, &sizeofsockaddr);
	if (client_created_fd == ERROR_SERVER)
		return (loop_errors_handlers_msg(ERROR_ACCEPT));
	else
	{
		client_created = new Clients(client_created_fd, *reinterpret_cast<struct sockaddr_in*>(&client_addr), "MyServerName");
		this->_clients_list.insert(std::make_pair(client_created, client_created_fd));
		this->_new_fd_nb = client_created_fd;
		std::cout << YELLOW << "A new client connected to the server. He holds the fd n° " << WHITE << client_created_fd << NORMAL << std::endl;
		this->_nb_of_clients++;
	}
}



std::vector<std::string> split(char *str, const char *delim)
{
	std::vector<std::string> vector;

	if (str == NULL)
		return vector;

	char 	*ptr = strtok(str, delim);

	while (ptr)
	{
		vector.push_back(std::string(ptr));
		ptr = strtok(NULL, delim);
	}
	return vector;
}



void		MyServer::RecvClientsMsg( int ClientsFd )
{
	
	char		recv_buffer[999999 + 1];
	char		*msg_buffer;
	int			ret_rcv;
	std::vector<std::string>	splitted_msg;
	std::vector<std::string>::iterator it;

	memset(recv_buffer, 0, 999999 + 1);
	ret_rcv = recv(ClientsFd, recv_buffer, 999999, MSG_DONTWAIT);
	if (ret_rcv == ERROR_SERVER)
		return (loop_errors_handlers_msg(ERROR_RECV));
	GetClientsThroughSocketFd(ClientsFd)->SetClientsMessage(recv_buffer);

	msg_buffer = strdup(GetClientsThroughSocketFd(ClientsFd)->GetClientsMessage().c_str());
	splitted_msg = split(msg_buffer, "\r\n");
	it = splitted_msg.begin();
	while (it != splitted_msg.end())
	{
		MyMsg new_msg(this->GetClientsThroughSocketFd(ClientsFd), *it);
		std::cout << WHITE << "You have a message : " << BLUE <<  *it << WHITE " from" << BLUE << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsNickname() << " socket n° " << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsFd() << std::endl;
		//std::cout << "Message from client #" << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsFd() << " (" << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsNickname() << ") << [" << *it << "]" << std::endl;
		//send(ClientsFd, recv_buffer, 999999, MSG_DONTWAIT);
		it++;
	}
	free(msg_buffer);
}

Clients		*MyServer::GetClientsThroughName( std::string NickName )
{
	std::map<Clients*, int>::iterator it;

	it = this->_clients_list.begin();
	while (it != this->_clients_list.end())
	{
		if (it->first->GetClientsNickname() == NickName)
			return (it->first);
		it++;
	}
	return (NULL);
}

Clients		*MyServer::GetClientsThroughSocketFd( int fd )
{
	std::map<Clients*, int>::iterator it;

	it = this->_clients_list.begin();
	while (it != this->_clients_list.end())
	{
		if (it->second == fd)
			return (it->first);
		it++;
	}
	return (NULL);
}