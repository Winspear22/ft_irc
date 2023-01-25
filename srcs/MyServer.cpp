# include "MyServer.hpp"
# include "Channels.hpp"
# include "Clients.hpp"
# include "MyMsg.hpp"


MyServer::MyServer( void )
{
	return ;
}

MyServer::MyServer( int port, std::string password ): _port(port), _password(password), _server_status(SERVER_ON)
{
	int i;
	std::string cmd_list_string[85] = {"ADMIN", "AWAY", "CNOTICE", "CPRIVMSG", "CONNECT", "DIE", "ENCAP", \
	"ERROR", "HELP", "INFO", "INVITE", "ISON", "JOIN", "KICK", "kill", "KNOCKS", "LINKS", "LIST", \
	"LUSERS", "MODE", "motd", "MOTD", "NAMES", "NICK", "NOTICE", "OPER", "PART", "PASS", "PING", \
	"PONG", "PRIVMSG", "QUIT", "REHASH", "RULES", "SERVER", "SERVICE", "SERVLIST", "SQUERY", \
	"SQUIT", "SETNAME", "SILENCE", "STATS", "SUMMON", "time", "TYPE", "TOPIC", "TRACE", "USER", "USERHOST", \
	"USERIP", "USERS", "version", "wallops", "VERSION", "userhost", "WALLOPS", "WATCH", "WHO", "WHOIS", "WHOWAS", "CAP", "info" };

	i = -1;
	while (++i < 85)
		this->_cmd_list.push_back(cmd_list_string[i]);
	this->_fds_list = 0;
	return ;
}

MyServer::MyServer( const MyServer & copy )
{
	*this = copy;
    return ;
}

MyServer::~MyServer( void )
{
	std::map<Clients*, int>::iterator it;
	std::map<Channels*, std::string>::iterator itt;
	
	this->MyServerDestructorMsg();
	it = this->clients_list.begin();	
	while (it != this->clients_list.end())
	{
		FD_CLR(it->second, &this->ready_fds);
		close(it->second);
		delete it->first;
		it++;
	}
	this->clients_list.clear();
	itt = this->channels_list.begin();
	while (itt != this->channels_list.end())
	{
		delete itt->first;
		itt++;
	}
	this->channels_list.clear();
	close(this->GetSocketFd());
	return ;
}

MyServer & MyServer::operator=( MyServer const & rhs )
{
	if ( this != &rhs )
    {
		this->new_msg = rhs.new_msg;
		this->clients_list = rhs.clients_list;
		this->channels_list = rhs.channels_list;
		this->ready_fds = rhs.ready_fds;
		this->readfds = rhs.readfds;
        this->_password = rhs._password;
		this->_port = rhs._port;
		this->_server_status = rhs._server_status;
		this->_socketfd = rhs._socketfd;
		this->_sockadress = rhs._sockadress;
		this->_right_password_used = rhs._right_password_used;
		this->_new_fd_nb = rhs._new_fd_nb;
		this->_nb_of_clients = rhs._nb_of_clients;
		this->_fds_list = rhs._fds_list;
		this->_maximum_fds = rhs._maximum_fds;
		this->_cmd_list = rhs._cmd_list;
		this->_it_cmd = rhs._it_cmd;
		this->_unavailable_nicknames = rhs._unavailable_nicknames;
    }
	return (*this);
}

/*----------------------------ALL THE SETTERS --------------------------*/

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

void		MyServer::SetCurrentClientsNb( int CurrentNb )
{
	this->_nb_of_clients = CurrentNb;
}
/*------------------------------ END OF SETTERS -----------------------------*/

/*------------------------------ ALL THE GETTERS ------------------------------*/

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

int			MyServer::GetCurrentClientsNb( void )
{
	return (this->_nb_of_clients);
}

/*-----------------------------------------------------------END OF GETTERS ---------------------------------------------*/

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
	const int 	optname = 1;

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

	ret = bind(this->_socketfd, (struct sockaddr *)&this->_sockadress, sizeof(this->_sockadress));
	if (ret == ERROR_SERVER)
	{
		close(this->_socketfd);
		return (ERROR_SOCKET_BINDING);
	}
	return (SUCCESS);
}

int			MyServer::ListenToSockedFd( void )
{
	int ret;

	ret = listen(this->_socketfd, 10);
	if (ret == ERROR_SERVER)
		return (ERROR_LISTENING);
	this->_maximum_fds = this->_socketfd;
	return (SUCCESS);
}

int			MyServer::SetSocketFdToNonBlocking( int SocketFd )
{
	int ret;
	ret = fcntl(SocketFd, F_SETFL, O_NONBLOCK);
	if (ret == ERROR_SERVER)
		return (ERROR_NONBLOCKING);
	return (SUCCESS);
}

int			MyServer::SelectClients( void )
{
	int					ret_select;
	struct timeval		timeout;

	memset(&timeout, 0, sizeof(struct timeval));
	timeout.tv_usec = 2000;
	this->_fds_list = 0;
	FD_ZERO(&this->ready_fds);
	FD_SET(this->_socketfd, &this->ready_fds);
	this->readfds = this->ready_fds;
	ret_select = select(this->_maximum_fds + 1, &this->readfds, NULL, NULL, &timeout);
	if (ret_select == ERROR_SERVER)
		loop_errors_handlers_msg(ERROR_SELECT);
	if (ret_select == TIMEOUT)
		loop_errors_handlers_msg(TIMEOUT);
	while (this->_fds_list <= this->_maximum_fds) 
	{
		if (FD_ISSET(this->_fds_list, &this->readfds)) 
		{
			this->CreateClients();
			FD_SET(this->_new_fd_nb, &this->ready_fds);
			if (this->_new_fd_nb > this->_maximum_fds)
         		this->_maximum_fds = this->_new_fd_nb;

		}
		else
			RecvClientsMsg(this->_fds_list);
		this->_fds_list++;
	}
	this->DeleteAFKClients();
	this->DeleteChannelsWithoutClients();
	this->deleteUnavailableNickname();
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
		this->clients_list.insert(std::make_pair(client_created, client_created_fd));
		this->SetCurrentClientsNb(this->GetCurrentClientsNb() + 1);
		if (this->GetCurrentClientsNb() > 8)
		{
			client_created->SetClientsConnectionStatus(NO);
			return ;
		}
		this->_new_fd_nb = client_created_fd;
	}
}

int			MyServer::DeleteAFKClients( void )
{
	std::map<Clients*, int>::iterator	it;

	it = this->clients_list.begin();
	if (it == this->clients_list.end())
		return (SUCCESS);
	while (it != this->clients_list.end())
	{
		if ((it->first->GetClientsConnectionStatus() == NO) || 
		(it->first->GetClientsLastPing() >= 120))
		{
			it->first->SetClientsConnectionStatus(NO);
			MyMsg msg(it->first, "QUIT :Client disconnected.");
			msg.parse_msg();
			msg.QuitCmd(this);
			it = this->clients_list.begin();
			if (it == this->clients_list.end())
				return (SUCCESS);
		}
		it++;
	}
	return (SUCCESS);
}

int			MyServer::DeleteChannelsWithoutClients( void )
{
	std::map<Channels *, std::string>::iterator it;

	it = this->channels_list.begin();
	while (it != this->channels_list.end())
	{
		if (it->first->GetAllClientsInChannelMemberList().empty())
		{
			if (it->first != NULL && this->GetChannelsByName(it->second) != NULL)
			{
				delete this->GetChannelsByName(it->second);
				this->channels_list.erase(it->first);
				break ;
			}
			it = this->channels_list.begin();
			if (it == this->channels_list.end())
				return (SUCCESS);
		}
		it++;
	}
	return (SUCCESS);
}

std::vector<std::string> MyServer::SplitByEndline(char *str, const char *delim)
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
	char								recv_buffer[512 + 1];
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
	std::string buf_str(recv_buffer);
	if (ret_rcv == ERROR_SERVER)
		return (loop_errors_handlers_msg(ERROR_RECV));
	else if (ret_rcv == ERROR_USER_DISCONNECTED)
		this->GetClientsThroughSocketFd(ClientsFd)->SetClientsConnectionStatus(NO); 
	else if (ret_rcv > 0 && (buf_str.rfind("\r") != buf_str.size() - 1) && (buf_str.rfind("\n") != buf_str.size() - 1))
	{
		std::cout << WHITE << "Incomplete new message : " << BLUE <<  buf_str << WHITE " from " << BLUE << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsNickname() << " socket n° " << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsFd() \
		<< CYAN << " Adding it to the buffer." << NORMAL << std::endl;
		this->GetClientsThroughSocketFd(ClientsFd)->SetClientsBuffer(GetClientsThroughSocketFd(ClientsFd)->GetClientsBuffer() + recv_buffer);
	}
	else if (this->GetClientsThroughSocketFd(ClientsFd) != NULL)
	{
		this->GetClientsThroughSocketFd(ClientsFd)->SetClientsMessage(this->GetClientsThroughSocketFd(ClientsFd)->GetClientsBuffer());
		this->GetClientsThroughSocketFd(ClientsFd)->SetClientsBuffer(GetClientsThroughSocketFd(ClientsFd)->GetClientsBuffer() + recv_buffer);
		msg_buffer = strdup(GetClientsThroughSocketFd(ClientsFd)->GetClientsBuffer().c_str());
		splitted_msg = this->SplitByEndline(msg_buffer, "\r\n");
		it = splitted_msg.begin();
		this->GetClientsThroughSocketFd(ClientsFd)->SetClientsMessage(this->GetClientsThroughSocketFd(ClientsFd)->GetClientsBuffer());
		this->GetClientsThroughSocketFd(ClientsFd)->SetClientsBuffer("");
		while (it != splitted_msg.end())
		{	
			this->new_msg = new MyMsg(this->GetClientsThroughSocketFd(ClientsFd), *it);
			std::cout << WHITE << "You have a message : " << BLUE <<  *it << WHITE " from " << BLUE << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsNickname() << " socket n° " << this->GetClientsThroughSocketFd(ClientsFd)->GetClientsFd()  << NORMAL << std::endl;
			tmp = strdup(it->c_str());
			tab_parse = this->SplitByEndline(tmp, " ");
			free(tmp);
			str = tab_parse.begin();
			if (str->at(0) == ':')
			{
				this->new_msg->Prefix = *str; 
				str++;
			}
			if (this->new_msg->CheckFormatCmd(str, this->_cmd_list) == SUCCESS)
			{
				this->new_msg->Command = *str; 
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
				str++;
			}
			if (this->new_msg->GetCmdExistence() == CMD_EXISTS)
			{
				this->_it_cmd = this->_cmd_list.begin();
				while (this->_it_cmd != this->_cmd_list.end())
				{
					if (*this->_it_cmd == this->new_msg->Command)
						this->CheckClientsAuthentification(this->new_msg->Command, this->new_msg);
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
	if (cmd == "PASS")
		this->ExecuteCommand(cmd, msg);
	else if (cmd == "NICK" && msg->GetClients()->GetClientsConnectionAuthorisation() == YES)
		this->ExecuteCommand(cmd, msg);
	else if (cmd == "USER" && msg->GetClients()->GetClientsConnectionAuthorisation() == YES)
		this->ExecuteCommand(cmd, msg);
	else if (msg->GetClients()->GetClientsConnectionPermission() == YES)
		this->ExecuteCommand(cmd, msg);
}

void		MyServer::ExecuteCommand( std::string cmd, MyMsg *msg)
{
	if (cmd == "PASS")
		msg->PassCmd(this);
	else if (cmd == "NICK")
		msg->NickCmd(this);
	else if (cmd == "USER" || msg->Command == "userhost")
		msg->UserCmd(this);
	else if (cmd == "motd")
		msg->MotdCmd();
	else if (cmd == "MODE")
		msg->ModeCmd(this);
	else if (cmd == "PING")
		msg->PingCmd(this);
	else if (cmd == "QUIT")
		msg->QuitCmd(this);
	else if (cmd == "version")
		msg->VersionCmd();
	else if (cmd == "PRIVMSG")
		msg->PrivMsgCmd(this);
	else if (cmd == "NOTICE")
		msg->NoticeCmd(this);
	else if (cmd == "JOIN")
		msg->JoinCmd(this);
	else if (cmd == "info")
		msg->InfoCmd();
	else if (cmd == "NAMES")
		msg->NamesCmd(this, *msg);
	else if (cmd == "LIST")
		msg->ListCmd(this);
	else if (cmd == "KICK")
		msg->KickCmd(this);
	else if (cmd == "PART")
		msg->PartCmd(this);
	else if (cmd == "wallops")
		msg->WallopsCmd(this);
	else if (cmd == "OPER")
		msg->OperCmd(this);
	else if (cmd == "TOPIC")
		msg->TopicCmd(this);
	else if (cmd == "kill")
		msg->KillCmd(this);
	else if (cmd == "INVITE")
		msg->InviteCmd(this);
	else if (cmd == "time")
		msg->TimeCmd(this);
}

void		SendMsgBackWithPrefix( MyMsg ClientMsg, std::string Msg )
{
	Msg = ClientMsg.GetPrefix() + " " + Msg;
	SendMsgBackToClients(ClientMsg, Msg);
}

void		SendMsgBackToClients( MyMsg ClientMsg, std::string Msg )
{
	int ret_send;

	if (ClientMsg.GetClients()->GetClientsConnectionAuthorisation() == YES)
	{
		Msg += "\r\n";
		std::cout << GREEN << "To client: " << WHITE << Msg << NORMAL;
		ret_send = send(ClientMsg.GetClients()->GetClientsFd(), Msg.c_str(), strlen(Msg.c_str()), MSG_DONTWAIT);
		if (ret_send == ERROR_SERVER)
			return (loop_errors_handlers_msg(ERROR_SEND));
	}
}

Clients		*MyServer::GetClientsThroughName( std::string NickName )
{
	std::map<Clients*, int>::iterator it;

	it = this->clients_list.begin();
	while (it != this->clients_list.end())
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

	it = this->clients_list.begin();
	while (it != this->clients_list.end())
	{
		if (it->second == fd)
			return (it->first);
		it++;
	}
	return (NULL);
}

Channels	*MyServer::GetChannelsByName( std::string ChannelName )
{
	std::map<Channels*, std::string>::iterator it;

	it = this->channels_list.begin();
	while (it != this->channels_list.end())
	{
		if (it->first->_ChannelName == ChannelName && it->second == ChannelName)
			return (it->first);
		it++;
	}
	return (NULL);
}


Channels		*MyServer::CreateChannels( std::string Channelname, Clients *client )
{
	std::map<Channels *, std::string>::iterator it;
	Channels *NewChannel = new Channels(client, Channelname);

	it = this->channels_list.begin();
	if (it == this->channels_list.end())
	{
		this->channels_list.insert(std::make_pair(NewChannel, NewChannel->_ChannelName));
		return (NewChannel);
	}
	else
	{
		while (it != this->channels_list.end())
		{
			if (Channelname == it->first->_ChannelName)
			{
				delete NewChannel;
				return (NULL);
			}
			it++;
		}
	}
	this->channels_list.insert(std::make_pair(NewChannel, NewChannel->_ChannelName));
	return (NewChannel);
}

void	MyServer::MyServerDestructorMsg( void )
{
	std::map<Clients*, int>::iterator	it;
	std::string							msg;

	it = this->clients_list.begin();
	msg = "QUIT :Server shutdown";
	if (it != this->clients_list.end())
		return ;
	while (it != this->clients_list.end())
	{
		MyMsg msg_sent(it->first, msg);
		SendMsgBackWithPrefix(msg_sent, msg);
		it++;
	}
}

int		MyServer::isUnavailableNickname(std::string nickname)
{
	std::map<std::string, clock_t>::iterator	it;


	it = this->_unavailable_nicknames.begin();
	while (it != this->_unavailable_nicknames.end())
	{
		if (it->first == nickname)
		{
			return 1;
		}
		it++;
	}
	return SUCCESS;
}

int		MyServer::SetUnavailableNickname(std::string nickname)
{
	clock_t	time;

	time = clock();
	this->_unavailable_nicknames.insert(std::make_pair(nickname, time));

	return SUCCESS;
}

void		MyServer::deleteUnavailableNickname( void )
{
	std::map<std::string, clock_t>::iterator	it;

	if (!this->_unavailable_nicknames.empty())
	{
		it = this->_unavailable_nicknames.begin();
		while (it != this->_unavailable_nicknames.end())
		{
			if (clock() - it->second >= DELAY)
			{
				std::cout << "Nickname " << GREEN << it->first << " is now available.\n";
				this->_unavailable_nicknames.erase(it);
				break;
			}
			it++;
		}
	}
}