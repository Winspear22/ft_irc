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
/*ON CHECK EN PREMIER SI LE PASS EST VIDE
EN SECOND SI LE PASS EST MAUVAIS
EN TROISIEME SI LE PASSE A DEJA ETE RENSEIGNE
ET ENFIN ON VALIDE SI TOUT VA BIEN*/
int		MyMsg::PassCmd( MyServer *IRC_Server )
{
	std::string msg_sent;
	std::vector<std::string>::iterator it;
	it = this->Params.begin();
	if (this->Params.empty() && this->_SentFrom->GetClientsConnectionAuthorisation() == NO) // Pas sûr de la deuxième condition
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		std::cout << RED << msg_sent << NORMAL << std::endl;
		this->_SentFrom->SetClientsConnectionAuthorisation(YES);
		SendMsgBackToClients(*this, msg_sent);
		this->_SentFrom->SetClientsConnectionAuthorisation(NO);
	}
	else if (*it != IRC_Server->GetPassword())
	{
		msg_sent = ERR_PASSWDMISMATCH();
		std::cout << RED << msg_sent << NORMAL << std::endl;
		this->_SentFrom->SetClientsConnectionAuthorisation(YES);
		SendMsgBackToClients(*this, msg_sent);
		this->_SentFrom->SetClientsConnectionAuthorisation(NO);
	}	
	else if (!this->Params.empty() && this->_SentFrom->GetClientsConnectionAuthorisation() == YES)
	{
		msg_sent = ERR_ALREADYREGISTRED(*this);
		std::cout << RED << msg_sent << NORMAL << std::endl;
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (*it == IRC_Server->GetPassword())
	{
		std::cout << "Pass validé" << std::endl;
		this->_SentFrom->SetClientsConnectionAuthorisation(YES);
	}
	return (SUCCESS);
}

int	MyMsg::NickFormatCheck( std::vector<std::string>::iterator nickcheck )
{
	unsigned int i;
	int j;
	char special_characters[9] = {'-', '_', '[', ']', '{', '}', '\\', '\'', '|'};
	
	i = 1;
	j = -1;
	while (special_characters[++j]) // Je check si le premier est un character spé
	{
		if (nickcheck->at(0) != special_characters[j] && !isalpha(nickcheck->at(0)))
			return (FAILURE);
	}
	while (i < nickcheck->size()) // je check si les 8 autres caractères sont autre choses qu'un alphanum + spé
	{
		j = -1;
		while (special_characters[++j])
		{
			if (nickcheck->at(i) != special_characters[j] && !isalnum(nickcheck->at(i)))
				return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

/*LA COMMANDE NICK QUI INITIALISE LE NICNAME*/
/*PAS ENCORE TERMINE IL RESTE A DETERMINE LA DERNIERE ETAPE CELLE AVEC 
UN CHANNEL*/
int	MyMsg::NickCmd( MyServer *IRC_Server )
{
	std::string msg_sent;
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator nick_format_check;
	it = this->Params.begin();
	nick_format_check = this->Params.begin();
	

	if (this->Params.empty()) // Est-ce que rien n'a été mis --> NICK ""
	{
		msg_sent = ERR_NONICKNAMEGIVEN();
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (this->Params.size() > 1) // Est-ce que plrs params ont été mis --> NICK popo lolo
	{
		msg_sent = ERR_NONICKNAMEGIVEN();
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (it->size() > 9 || it->size() == 0) // La taille du NICK est plus grand que 9 ou vide --> NICK lolololololo OU NICK ""
	{
		msg_sent = ERR_ERRONEUSNICKNAME(*this);
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (this->NickFormatCheck(nick_format_check) == FAILURE) // Format illégal du NICK --> NICK 1popo 
	{
		msg_sent = ERR_ERRONEUSNICKNAME(*this);
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (IRC_Server->GetClientsThroughName(*it) != NULL) // Verifier si le pseudo existe ou non --> NICK user42 NICK user42
	{
		std::cout << "ICI" << std::endl;
		msg_sent = ERR_NICKNAMEINUSE(*this);
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (IRC_Server->GetClientsThroughName(*it) == NULL) // Tout va bien, alors je peux remplir le Nickname
	{
		this->_SentFrom->SetClientsNickname(*it);
		msg_sent = "NICK ";
		msg_sent = msg_sent + *it;
		SendMsgBackToClients(*this, msg_sent);
		/*Si le NICK et le USER sont OK, alors tout est OK*/
		this->_SentFrom->SetClientsConnectionNickCmd(YES);
		if (this->_SentFrom->GetClientsConnectionUserCmd() == YES && this->_SentFrom->GetClientsConnectionNickCmd() == YES && this->_SentFrom->GetClientsConnectionAuthorisation() == YES)
			ValidateClientsConnections();
		return (SUCCESS);
	}
	/*IL RESTE UN CAS DE FIGURE, CELUI DES CHANNELS, MAIS AUCUNE IDEE DE QUOI FAIRE*/
	return (FAILURE);
}

/*LA COMMANDE NICK QUI SET LE USERNAME, LE HOSTNAME ET LE REALNAME 
ELLE EST FAIE A LA ZOB MAIS ELLE MARCHE, C'EST L'IMPORTANT */
/*TROUVER COMMENT CHANGER LE USERNAME, HOSTNAME ET REALNAME DANS LES 
OPTIONS POUR FAIRE UN VRAI ALGORITHME*/
int	MyMsg::UserCmd( MyServer *IRC_Server )
{
	(void)IRC_Server;
	std::string msg_sent;
	std::string realname;
	std::vector<std::string>::iterator it;

	it = this->Params.begin();
	if (this->Params.size() < 4)
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (this->_SentFrom->GetClientsUsername().empty() == false)
	{
		msg_sent = ERR_ALREADYREGISTRED(*this);
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (this->Params.size() >= 4) // > 4 car le realname peut contenir des  espaces
	{
		while (it != this->Params.end())
		{
			if (it->empty() == true)
			{
				msg_sent = ERR_NEEDMOREPARAMS(*this);
				SendMsgBackToClients(*this, msg_sent);
			}
			it++;
		}
		it = this->Params.begin();
		this->_SentFrom->SetClientsUsername(*it);
		if (it->at(0) == ':')
			realname = it->substr(1);
		else
			realname = *it;
		while (it != this->Params.end())
		{
			realname = realname + " " + *it;
			std::cout << "realname == " << realname << std::endl;
			it++;
		}
		this->_SentFrom->SetClientsRealname(realname);
		this->_SentFrom->SetClientsConnectionUserCmd(YES);
	/*Si le NICK et le USER sont OK, alors tout est OK*/
		if (this->_SentFrom->GetClientsConnectionUserCmd() == YES && this->_SentFrom->GetClientsConnectionNickCmd() == YES && this->_SentFrom->GetClientsConnectionAuthorisation() == YES)
			ValidateClientsConnections();
	}
	return (SUCCESS);
}

int			MyMsg::ModeCmd( MyServer *IRC_Server )
{
	std::string msg_sent;

	msg_sent = "\033[1;35mMODE\033[0m";
	SendMsgBackToClients(*this, msg_sent);
	(void)IRC_Server;
	return (SUCCESS);
}

int			MyMsg::PingCmd( MyServer *IRC_Server )
{
	std::string msg_sent;

	msg_sent = "\033[1;35mPONG\033[0m";
	SendMsgBackToClients(*this, msg_sent);
	(void)IRC_Server;
	return (SUCCESS);
}


int			MyMsg::QuitCmd( MyServer *IRC_Server )
{
	unsigned int			i;
	std::string msg_sent;

	msg_sent = "Quit: "; //message renvoyé selon le RFC modern.ircdocs.horse
	i = -1;
	if (this->Params.size() == 0)
		msg_sent = "Quit: ";  //message renvoyé selon si le Client n'a pas spécifié de raison
	if (this->Params.size() >= 1)
	{
		while (++i < this->Params.size())
			msg_sent = msg_sent + " " + this->Params[i]; // message renvoyé si le client a spécifié une raison
	}
	//SendMsgBackToClients(*this, msg_sent);
	//IRC_Server->DeleteDisconnectedClients(this->_SentFrom->GetClientsFd());
	(void)IRC_Server;
	return (SUCCESS);
}


int		MyMsg::ValidateClientsConnections( void )
{
	std::string intro_new_nick;

	this->_SentFrom->SetClientsConnectionPermission(YES);
	std::cout << GREEN << "Client validé" << NORMAL << std::endl;
	SendMsgBackToClients(*this, ::RPL_WELCOME(*this));
	SendMsgBackToClients(*this, ::RPL_YOURHOST(*this));
	SendMsgBackToClients(*this, ::RPL_CREATED(*this));
	SendMsgBackToClients(*this, ::RPL_MYINFO(*this));
	SendMsgBackToClients(*this, "\r\n");

	intro_new_nick = "\033[1;35mIntroducing new nick \033[1;37m" + this->_SentFrom->GetClientsNickname() + "\n";
	SendMsgBackToClients(*this, intro_new_nick);
	return (SUCCESS);
}
