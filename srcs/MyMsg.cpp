# include "MyMsg.hpp"
# include <sys/socket.h>
# include <sys/types.h>
# include "num_replies.cpp"

MyMsg::MyMsg( void )
{
	//std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}


MyMsg::MyMsg( Clients *client, std::string MyMsg )
{
	this->_Message = MyMsg;
	this->_SentFrom = client;
	//std::cout << GREEN << "MyMsg Constructor called." << NORMAL << std::endl;
	return ;
}

MyMsg::MyMsg( const MyMsg & copy )
{
	//std::cout << "\033[0;33mMyMsg Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

MyMsg & MyMsg::operator=( MyMsg const & rhs )
{
	//std::cout << "\033[0;34mMyMsg Copy assignment operator called." << NORMAL << std::endl;
	this->_Message = rhs._Message;
	this->_SentFrom = rhs._SentFrom;
	return (*this);
}


MyMsg::~MyMsg( void )
{
	//std::cout << RED << "MyMsg Destructor called." << NORMAL << std::endl;
	return ;
}

std::string MyMsg::GetMsg( void )
{
	return (this->_Message);
}

std::string MyMsg::GetPrefix( void )
{
	return (this->_Prefix);
}

std::string MyMsg::GetCmd( void )
{
	return (this->_Command);
}

/*std::string MyMsg::GetParams( void )
{
	return (this->_Params);
}*/

Clients	*MyMsg::GetClients( void )
{
	return (this->_SentFrom);
}

void MyMsg::SetPrefix( std::string Prefix)
{
	this->Prefix = Prefix;
}

std::string toupper_striing( std::string cmd )
{
	int i;
	std::string capitalized_str;

	i = -1;
	while (cmd[++i])
		capitalized_str[i] = toupper(cmd[i]);
	return (capitalized_str);
}

void MyMsg::SetCmd( std::string Cmd )
{
	this->Command = toupper_striing( Cmd );
}

int	MyMsg::GetCmdExistence( void )
{
	return (this->_DoesCmdExist);
}

void	MyMsg::SetCmdExistence( int CmdStatus )
{
	this->_DoesCmdExist = CmdStatus;
}

void MyMsg::SetParams( std::string Params )
{
	this->Params.push_back(Params);
}

int			MyMsg::CheckFormatCmd( std::string cmd, std::vector<std::string> cmd_list )
{
	std::vector<std::string>::iterator it;
	
	it = cmd_list.begin();
	while (it != cmd_list.end())
	{
		if (*it == cmd)
			return (SUCCESS);
		it++;
	}
	return (SUCCESS);
}

/*LA COMMANDE PASS QUI VERIFIE LA VERACITE DU PASS*/
int		MyMsg::PassCmd( void )
{
	std::string msg;
	std::vector<std::string>::iterator it;
	it = this->Params.begin();
	if (it->size() < 1)
	{
		msg = "\033[1;31mERR_NEEDMOREPARAMS \033[1;37mPASS :Not enough parameters\n\033[0m";
		SendMsgBackToClients(*this, msg);
	}
	else if (*it != "111")
	{
		msg = "\033[1;31mERR_PASSWDMISMATCH \033[1;37mPASS :Password incorrect\n\033[0m";
		SendMsgBackToClients(*this, msg);
	}	
	else if (it->size() >= 1)
	{
		if (this->_SentFrom->GetClientsConnectionPermission() == YES)
		{
			msg = "\033[1;31mERR_ALREADYREGISTERED \033[1;37mPASS :You may not reregister\n\033[0m";
			SendMsgBackToClients(*this, msg);
		}
	}
	return (SUCCESS);
}

/*LA COMMANDE NICK QUI INITIALISE LE NICNAME*/
int	MyMsg::NickCmd( void )
{
	std::vector<std::string>::iterator it;
	it = this->Params.begin();
	if (this->Params.size() >= 1)
		this->_SentFrom->SetClientsNickname(*it);
	return (SUCCESS);
}

/*LA COMMANDE NICK QUI SET LE USERNAME, LE HOSTNAME ET LE REALNAME 
ELLE EST FAIE A LA ZOB MAIS ELLE MARCHE, C'EST L'IMPORTANT */
/*TROUVER COMMENT CHANGER LE USERNAME, HOSTNAME ET REALNAME DANS LES 
OPTIONS POUR FAIRE UN VRAI ALGORITHME*/
int	MyMsg::UserCmd( void )
{
	std::string msg_sent;
	std::string username;
	std::string hostname;
	std::string realname;
	std::vector<std::string>::iterator it;

	it = this->Params.begin();
	username = *it;
	this->_SentFrom->SetClientsUsername(username);
	it++;
	hostname = *it;
	it++;
	hostname += " " + *it;
	this->_SentFrom->SetClientsHostname(hostname);
	it++;
	realname = *it;
	it++;
	if (it != this->Params.end())
	{
		while (it != this->Params.end())
		{
			realname += " " + *it;
			it++;
		}
	}
	this->_SentFrom->SetClientsRealname(realname);
	return (SUCCESS);
}

int			MyMsg::ModeCmd( void )
{
	std::string msg_sent;

	msg_sent = "MODE";
	SendMsgBackToClients(*this, msg_sent);
	return (SUCCESS);
}

int			MyMsg::PingCmd( void )
{
	std::string msg_sent;

	msg_sent = "PONG";
	SendMsgBackToClients(*this, msg_sent);
	return (SUCCESS);
}

int		MyMsg::ValidateClientsConnections( void )
{
	std::string intro_new_nick;


	SendMsgBackToClients(*this, ::RPL_WELCOME(*this));
	SendMsgBackToClients(*this, ::RPL_YOURHOST(*this));
	SendMsgBackToClients(*this, ::RPL_CREATED(*this));
	SendMsgBackToClients(*this, ::RPL_MYINFO(*this));
	SendMsgBackToClients(*this, "\r\n");
	this->_SentFrom->SetClientsConnectionPermission(YES);

	intro_new_nick = "\033[1;35mIntroducing new nick \033[1;37m" + this->_SentFrom->GetClientsNickname() + "\n";
	SendMsgBackToClients(*this, intro_new_nick);
	return (SUCCESS);
}
