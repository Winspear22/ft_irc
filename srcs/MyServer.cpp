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
	std::string cmd_list_string[80] = {"ADMIN", "AWAY", "CNOTICE", "CPRIVMSG", "CONNECT", "DIE", "ENCAP", \
	"ERROR", "HELP", "INFO", "INVITE", "ISON", "JOIN", "KICK", "KILL", "KNOCKS", "LINKS", "LIST", \
	"LUSERS", "MODE", "motd", "MOTD", "NAMES", "NICK", "NOTICE", "OPER", "PART", "PASS", "PING", \
	"PONG", "PRIVMSG", "QUIT", "REHASH", "RULES", "SERVER", "SERVICE", "SERVLIST", "SQUERY", \
	"SQUIT", "SETNAME", "SILENCE", "STATS", "SUMMON", "TYPE", "TOPIC", "TRACE", "USER", "USERHOST", \
	"USERIP", "USERS", "VERSION", "WALLOPS", "WATCH", "WHO", "WHOIS", "WHOWAS", "CAP" };

	i = -1;
	while (++i < 80)
		this->_cmd_list.push_back(cmd_list_string[i]);
	this->_fds_list = 0;
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

int				MyServer::GetSocketFd( void )
{
	return (this->_socketfd);
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
	this->_maximum_fds = this->_socketfd;
	return (SUCCESS);
}

int			MyServer::SetSocketFdToNonBlocking( int SocketFd )
{
	int ret;
	ret = fcntl(SocketFd, F_SETFL, O_NONBLOCK);
	if (ret == ERROR_SERVER)
		return (ERROR_NONBLOCKING);
	std::cout << GREEN << "fcntl(); works !" << std::endl;
	return (SUCCESS);
}

int			MyServer::SelectClients( void )
{
	fd_set	ready_fds; //mes fds etant prets a transmettre des donnes
	fd_set	readfds; // mes sets de fds pouvant lire
	//int		maximum_fds; // mon nombre max de fds peut etre remplace par FD_ISSET
	int		ret_select; //return de select pour les erreurs
//	int		fds_list; // sert a loop pour trouver lequel des fds a des donnees pour moi
	struct timeval		timeout;


	memset(&timeout, 0, sizeof(struct timeval));
	timeout.tv_usec = 200000;
	this->_fds_list = 0;
	FD_ZERO(&ready_fds);
	FD_SET(this->_socketfd, &ready_fds);
	readfds = ready_fds; // je sais plus pk on fait ca 
	ret_select = select(this->_maximum_fds + 1, &readfds, NULL, NULL, &timeout);
	if (ret_select == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_SELECT);
	if (ret_select == TIMEOUT)
		loop_errors_handlers_msg(TIMEOUT);
	while (this->_fds_list <= this->_maximum_fds) //on doit checker ce qui se passe sur tous les fds un par un
	{
		if (FD_ISSET(this->_fds_list, &readfds)) // C'est un client qui a ete trouve
		{
			this->CreateClients();
			FD_SET(this->_new_fd_nb, &ready_fds);
			if (this->_new_fd_nb > this->_maximum_fds)
         	     this->_maximum_fds = this->_new_fd_nb;
			std::cout << CYAN << "Client currently connected : " << this->_nb_of_clients << std::endl;
		}
		else
			RecvClientsMsg(this->_fds_list);
		this->_fds_list++;
	}
	return (SUCCESS);
}

void			MyServer::CreateClients( void )
{
	int				client_created_fd;
	Clients			*client_created;
	struct sockaddr	client_addr;
	socklen_t		sizeofsockaddr;
	int				ret_fcntl;
	
	sizeofsockaddr = sizeof(client_addr);
	client_created_fd = accept(this->_socketfd, &client_addr, &sizeofsockaddr);
	ret_fcntl = SetSocketFdToNonBlocking(client_created_fd);
	if (ret_fcntl == ERROR_SERVER)
		return (loop_errors_handlers_msg(ERROR_NONBLOCKING));
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
	std::string 						cmd;
	char								*tmp;

	memset(recv_buffer, 0, 512 + 1);
	ret_rcv = recv(ClientsFd, recv_buffer, 512, MSG_DONTWAIT);
	if (ret_rcv == ERROR_SERVER)
		return (loop_errors_handlers_msg(ERROR_RECV));
	if (ret_rcv == ERROR_USER_DISCONNECTED && this->GetClientsThroughSocketFd(ClientsFd) == NULL)
		return ;  // Cas où le client se deconnecte normalement, dans le cas où recv n'a rien reçu de la part d'un fd
	else if (ret_rcv != ERROR_USER_DISCONNECTED && this->GetClientsThroughSocketFd(ClientsFd) != NULL && this->GetClientsThroughSocketFd(ClientsFd)->GetClientsConnectionStatus() == YES)
	{
		GetClientsThroughSocketFd(ClientsFd)->SetClientsMessage(recv_buffer);
		msg_buffer = strdup(GetClientsThroughSocketFd(ClientsFd)->GetClientsMessage().c_str());
		splitted_msg = SplitByEndline(msg_buffer, "\r\n");
		it = splitted_msg.begin();
		while (it != splitted_msg.end())
		{	
			this->new_msg = new MyMsg(this->GetClientsThroughSocketFd(ClientsFd), *it);
			std::cout << WHITE << "You have a message : " << BLUE <<  *it << WHITE " from " << BLUE << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsNickname() << " socket n° " << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsFd()  << NORMAL << std::endl;
			tmp = strdup(it->c_str());
			tab_parse = SplitByEndline(tmp, " ");
			free(tmp);
			str = tab_parse.begin();
			if (str->at(0) == ':')
			{
				//this->new_msg->SetPrefix(*str);
				this->new_msg->Prefix = *str; //VERIFIER QUE LE SETTER MARCHE
				str++;
			}
			if (this->new_msg->CheckFormatCmd(str, this->_cmd_list) == SUCCESS)
			{
				this->new_msg->Command = *str; //essayer de trouver un moyen d'utiliser un setter
				str++;
				this->new_msg->SetCmdExistence(CMD_EXISTS);
			}
			else
			{
				std::cout << RED << "Error. The command format you wrote is wrong. You need one letter followed by three numbers." << std::endl;
				this->new_msg->SetCmdExistence(CMD_DOESNT_EXIST);
			}
			while (str != tab_parse.end())
			{
				this->new_msg->SetParams(*str);
				//this->new_msg->Params.push_back(*str);
				str++;
			}
			if (this->new_msg->GetCmdExistence() == CMD_EXISTS)
			{
				this->_it_cmd = this->_cmd_list.begin();
				while (this->_it_cmd != this->_cmd_list.end())
				{
					if (*this->_it_cmd == this->new_msg->Command)
						this->CheckClientsAuthentification(*this->_it_cmd, this->new_msg);
					this->_it_cmd++;
				}
			}
			delete this->new_msg;
			it++;
		}
		free(msg_buffer);
	}
}

void		MyServer::CheckClientsAuthentification( std::string cmd, MyMsg *msg )
{
	if (cmd == "PASS" || cmd == "NICK" || cmd == "USER" || msg->GetClients()->GetClientsConnectionPermission() == YES)
		this->ExecuteCommand(cmd, msg);
}

void		MyServer::ExecuteCommand( std::string cmd, MyMsg *msg)
{
	if (cmd == "PASS")
		msg->PassCmd(this);
	else if (cmd == "NICK")
		msg->NickCmd(this);
	else if (cmd == "USER")
		msg->UserCmd(this);
	else if (cmd == "motd")
		msg->MotdCmd();
	else if (cmd == "MODE")
		msg->ModeCmd(this);
	else if (cmd == "PING")
		msg->PingCmd(this);
	else if (cmd == "QUIT")
		msg->QuitCmd(this);
}

void		SendMsgBackToClients( MyMsg ClientMsg, std::string Msg )
{
	int ret_send;

	if (ClientMsg.GetClients()->GetClientsConnectionAuthorisation() == YES)
	{
		ret_send = send(ClientMsg.GetClients()->GetClientsFd(), Msg.c_str(), strlen(Msg.c_str()), MSG_DONTWAIT);
		if (ret_send == ERROR_SERVER)
			return (loop_errors_handlers_msg(ERROR_SEND));
	}
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

/*int	 MyServer::DeleteDisconnectedClients( void )
{
	std::map<Clients *, int>::iterator it;

	it = this->_clients_list.begin();
	while (it != this->_clients_list.end())
	{
		if (it->first->GetClientsConnectionStatus() == NO || it->first->)
		it++;
	}
	return (SUCCESS);
}*/