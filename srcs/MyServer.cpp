# include "MyServer.hpp"

MyServer::MyServer( void )
{
	std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}


MyServer::MyServer( int port, std::string password ): _port(port), _password(password), _server_status(SERVER_ON)
{
	std::cout << GREEN << "MyServer Constructor called." << NORMAL << std::endl;
	int i;
	std::string cmd_list_string[79] = {"ADMIN", "AWAY", "CNOTICE", "CPRIVMSG", "CONNECT", "DIE", "ENCAP", \
	"ERROR", "HELP", "INFO", "INVITE", "ISON", "JOIN", "KICK", "KILL", "KNOCKS", "LINKS", "LIST", \
	"LUSERS", "MODE", "MOTD", "NAMES", "NICK", "NOTICE", "OPER", "PART", "PASS", "PING", \
	"PONG", "PRIVMSG", "QUIT", "REHASH", "RULES", "SERVER", "SERVICE", "SERVLIST", "SQUERY", \
	"SQUIT", "SETNAME", "SILENCE", "STATS", "SUMMON", "TYPE", "TOPIC", "TRACE", "USER", "USERHOST", \
	"USERIP", "USERS", "VERSION", "WALLOPS", "WATCH", "WHO", "WHOIS", "WHOWAS", "CAP" };

	i = -1;
	while (++i < 79)
		this->_cmd_list.push_back(cmd_list_string[i]);
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
	this->_right_password_used = FAILURE;
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

int			MyServer::SelectClients( void )
{
	fd_set	ready_fds; //mes fds etant prets a transmettre des donnes
	fd_set	readfds; // mes sets de fds pouvant lire
	int		maximum_fds; // mon nombre max de fds peut etre remplace par FD_ISSET
	int		ret_select; //return de select pour les erreurs
	int		fds_list; // sert a loop pour trouver lequel des fds a des donnees pour moi
	struct timeval		timeout;

	memset(&timeout, 0, sizeof(struct timeval));
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
		loop_errors_handlers_msg(TIMEOUT);
	while (++fds_list <= maximum_fds) //on doit checker ce qui se passe sur tous les fds un par un
	{
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

std::vector<std::string> SplitByEndline(char *str, const char *delim)
{
	char 	*tmp;
	std::vector<std::string> splitted_str;

	if (str == NULL)
		return (splitted_str);
	tmp = strtok(str, delim);
	while (tmp)
	{
		splitted_str.push_back(std::string(tmp));
		tmp = strtok(NULL, delim);
	}
	return (splitted_str);
}

void		MyServer::RecvClientsMsg( int ClientsFd )
{
	
	char								recv_buffer[512 + 1]; // ON UTILISE 512 CAR C'EST LA LIM D'UN MESSAGE SELON LE RFC
	char								*msg_buffer;
	int									ret_rcv;
	std::vector<std::string>			splitted_msg;
	std::vector<std::string>::iterator 	it;
	
	std::vector<std::string>			tab_parse;
	std::vector<std::string>::iterator 	str;
	char								*tmp;


	memset(recv_buffer, 0, 512 + 1);
	ret_rcv = recv(ClientsFd, recv_buffer, 512, MSG_DONTWAIT);
	if (ret_rcv == ERROR_SERVER)
		return (loop_errors_handlers_msg(ERROR_RECV));
	GetClientsThroughSocketFd(ClientsFd)->SetClientsMessage(recv_buffer);

	msg_buffer = strdup(GetClientsThroughSocketFd(ClientsFd)->GetClientsMessage().c_str());
	splitted_msg = SplitByEndline(msg_buffer, "\r\n");
	it = splitted_msg.begin();
	while (it != splitted_msg.end())
	{
		MyMsg new_msg(this->GetClientsThroughSocketFd(ClientsFd), *it);
		std::cout << WHITE << "You have a message : " << BLUE <<  *it << WHITE " from" << BLUE << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsNickname() << " socket n° " << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsFd()  << NORMAL << std::endl;
		/*PARSING DU MESSAGE*/
		tmp = strdup(it->c_str());
		tab_parse = SplitByEndline(tmp, " ");
		free(tmp);
		str = tab_parse.begin();
		if (str->at(0) == ':')
		{
			new_msg.SetPrefix(*str);
			str++;
		}
		if (new_msg.CheckFormatCmd(*str, this->_cmd_list) == SUCCESS)
		{
			new_msg.SetCmd(*str);
			std::cout << new_msg.GetCmd() << std::endl;
			str++;
		}
		else
			std::cout << RED << "Error. The command format you wrote is wrong. You need one letter followed by three numbers." << std::endl;
		while (str != tab_parse.end())
		{
			new_msg.SetParams(*str);
			str++;
		}
		if (it->at(0) == 'P' && it->at(1) == 'A' && it->at(2) == 'S' && it->at(3) == 'S')
			new_msg.PassCmd();
		if (it->at(0) == 'N' && it->at(1) == 'I' && it->at(2) == 'C' && it->at(3) == 'K')
			new_msg.NickCmd();
		if (it->at(0) == 'U' && it->at(1) == 'S' && it->at(2) == 'E' && it->at(3) == 'R')
		{
			new_msg.UserCmd();
			new_msg.ValidateClientsConnections();
		}
		it++;
	}
	free(msg_buffer);
}

int		MyServer::ParsingOfClientsCmds( std::vector<std::string>::iterator msg_split_by_space_it, MyMsg msg, std::vector<std::string> msg_split_by_space )
{
	if (this->ParsingOfPrefix( msg_split_by_space_it, msg ) == FAILURE)
		return (FAILURE);
	if (this->ParsingOfCmd( msg_split_by_space_it, msg ) == FAILURE)
		return (FAILURE);
	if (this->ParsingOfParams( msg_split_by_space_it, msg, msg_split_by_space ) == FAILURE)
		return (FAILURE);
	(void)msg_split_by_space_it;
	(void)msg;
	(void)msg_split_by_space;
	return (SUCCESS);
}


int		MyServer::ParsingOfPrefix( std::vector<std::string>::iterator msg_split_by_space_it, MyMsg msg )
{
	if (msg_split_by_space_it->at(0) == ':')
	{
		msg.SetPrefix(*msg_split_by_space_it);
		msg_split_by_space_it++;
	}
	return (SUCCESS);
}

int		MyServer::ParsingOfCmd( std::vector<std::string>::iterator msg_split_by_space_it, MyMsg msg )
{
	if (msg.CheckFormatCmd(*msg_split_by_space_it, this->_cmd_list) == SUCCESS)
	{
		msg.SetCmd(*msg_split_by_space_it);
		msg_split_by_space_it++;
	}
	return (SUCCESS);
}

int		MyServer::ParsingOfParams( std::vector<std::string>::iterator msg_split_by_space_it, MyMsg msg, std::vector<std::string> msg_split_by_space )
{
	while (msg_split_by_space_it != msg_split_by_space.end())
	{
		std::cout << BLUE << "msg_split == " << *msg_split_by_space_it << NORMAL << std::endl;
		msg.SetParams(*msg_split_by_space_it);
		msg_split_by_space_it++;
	}
	(void)msg_split_by_space_it;
	(void)msg;
	(void)msg_split_by_space;
	return (SUCCESS);
}


void		SendMsgBackToClients( MyMsg ClientMsg, std::string Msg )
{
	int ret_send;

	ret_send = send(ClientMsg.GetClients()->GetClientsFd(), Msg.c_str(), strlen(Msg.c_str()), MSG_DONTWAIT);
	if (ret_send == ERROR_SERVER)
		return (loop_errors_handlers_msg(ERROR_SEND));
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

std::vector<std::string> MyServer::GetCmdList( void )
{
	return (this->_cmd_list);
}
