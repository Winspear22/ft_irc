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
	std::string tmp;

	if (!this->_SentFrom->GetClientsNickname().empty())
	{
		tmp = ":" + this->_SentFrom->GetClientsNickname();
		if (!this->_SentFrom->GetClientsUsername().empty())
			tmp = tmp + "!" + this->_SentFrom->GetClientsUsername();
		if (!this->_SentFrom->GetClientsUsername().empty())
			tmp = tmp + "@" + this->_SentFrom->GetClientsHostname();
		else
			tmp = tmp + "@" + this->_SentFrom->GetClientsHostAdress();
	}
	else
		tmp = ":" + this->_SentFrom->GetServerName();
	this->_Prefix = tmp;
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


/*verifier la pertinence de cette fct par rapport au RFC car il y'a un bail avec la grammaire des commandes : 1 chifres et 3 lettres*/
int			MyMsg::CheckFormatCmd( std::vector<std::string>::iterator cmd, std::vector<std::string> cmd_list )
{
	std::vector<std::string>::iterator it;
	
	it = cmd_list.begin();
	while (it != cmd_list.end())
	{
		if (*it == *cmd)
			return (SUCCESS);
		it++;
	}
	return (FAILURE);
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
	unsigned int 	i;
	int 			j;
	int				not_a_special_character;
	char 			special_characters[9] = {'-', '_', '[', ']', '{', '}', '\\', '\'', '|'};
	
	i = 1;
	j = -1;
	not_a_special_character = 0;
	while (special_characters[++j]) // Je check si le premier est un character spé
	{
		if (nickcheck->at(0) != special_characters[j])
		{
			not_a_special_character = 1;
			break ;
		}
		else
			not_a_special_character = 0;
	}
	if (not_a_special_character == 1 && !isalpha(nickcheck->at(0)))
		return (FAILURE);
	if (not_a_special_character == 0 && j == 8 && !isalpha(nickcheck->at(0)))
		return (FAILURE);
	
	not_a_special_character = 0;
	while (i < nickcheck->size()) // je check si les 8 autres caractères sont autre choses qu'un alphanum + spé
	{
		j = -1;
		while (special_characters[++j])
		{
			if (nickcheck->at(i) != special_characters[j])
			{
				not_a_special_character = 1;
				break ;
			}
			else
				not_a_special_character = 0;
		}
		if (not_a_special_character == 1 && j == 8 && !isalnum(nickcheck->at(i)))
		{	
			std::cout << RED << special_characters[j] << " = " << j << YELLOW << " = " << nickcheck->at(i) << NORMAL << std::endl;
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
	std::map<Channels*, std::string>::iterator channels_it;
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
		msg_sent = ERR_NICKNAMEINUSE(*this);
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (IRC_Server->GetClientsThroughName(*it) == NULL  && this->NickFormatCheck(nick_format_check) == SUCCESS && this->_SentFrom->GetClientsNickname().empty()) // Tout va bien, alors je peux remplir le Nickname
	{
		this->_SentFrom->SetClientsNickname(*it);
		msg_sent = "NICK ";
		msg_sent = msg_sent + *it + "\r\n"; // Ne pas oublier le \n pour signifier la fin du retour de cmd.
		SendMsgBackToClients(*this, msg_sent);
		/*Si le NICK et le USER sont OK, alors tout est OK*/
		this->_SentFrom->SetClientsConnectionNickCmd(YES);
		if (this->_SentFrom->GetClientsConnectionUserCmd() == YES && this->_SentFrom->GetClientsConnectionNickCmd() == YES && this->_SentFrom->GetClientsConnectionAuthorisation() == YES \
		&& this->_SentFrom->GetClientsConnectionPermission() == NO)
			ValidateClientsConnections();
		return (SUCCESS);
	}
	else if (IRC_Server->GetClientsThroughName(*it) == NULL && this->NickFormatCheck(nick_format_check) == SUCCESS && !this->_SentFrom->GetClientsNickname().empty())
	{
		std::cout << RED << "je suis la et nick == " << WHITE << *it << NORMAL << std::endl;

		msg_sent = "NICK ";
		msg_sent = msg_sent + *it + "\r\n";
		channels_it = IRC_Server->channels_list.begin();
		while (channels_it != IRC_Server->channels_list.end())
		{
			std::cout << "channels_it->second === " << channels_it->second << std::endl;
			if (channels_it->first->GetClientsInChannelMemberList(this->_SentFrom->GetClientsNickname()) != NULL)
			{	
				std::cout << RED << "Dans le if Channels de NICK" << NORMAL << std::endl;
				channels_it->first->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
			}
			channels_it++;
		}
		std::cout << RED << "je suis la et nick == " << WHITE << *it << NORMAL << std::endl;
		SendMsgBackWithPrefix(*this, msg_sent);
		this->_SentFrom->SetClientsNickname(*it);
		return (SUCCESS);
	}
	/*IL RESTE UN CAS DE FIGURE, CELUI DES CHANNELS, MAIS AUCUNE IDEE DE QUOI FAIRE*/
	return (FAILURE);
}

/*LA COMMANDE USER QUI SET LE USERNAME, LE mode ET LE REALNAME */
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
		it++;
		this->_SentFrom->SetClientsMode(*it); // A CE MOMENT LA IL FAUDRA LANCER LA COMMANDE MODE
		it++;
		this->_SentFrom->SetClientsUnused(*it);
		it++;
		while (it != this->Params.end())
		{
			if (it->empty()) // Si aucun realname n'est renseigné, il doit etre remplacé par le NICK
				realname = this->_SentFrom->GetClientsNickname();
			else // Sinon, tout ce qui suit après le 3e arguments = realname
			{	
				if (it->at(0) == ':') // S'il y'a plusieurs ':' les uns à la suite des autres, seul le premier ':' doit être évité
				{
					realname = it->substr(1);
					it++;
					while (it != this->Params.end())
					{
						realname += " " + *it;
						it++;
					}						
					break ;
				}
				else
					realname += " " + *it;
			}
			it++;
		}
		this->_SentFrom->SetClientsRealname(realname);
		this->_SentFrom->SetClientsConnectionUserCmd(YES);
	/*Si le NICK et le USER sont OK, alors tout est OK*/
		if (this->_SentFrom->GetClientsConnectionUserCmd() == YES && this->_SentFrom->GetClientsConnectionNickCmd() == YES && this->_SentFrom->GetClientsConnectionAuthorisation() == YES \
		&& this->_SentFrom->GetClientsConnectionPermission() == NO)
			ValidateClientsConnections();
	}
	return (SUCCESS);
}


/*MODE NON FONCTIONNEL ENCORE*/
int			MyMsg::ModeCmd( MyServer *IRC_Server )
{

	(void)IRC_Server;
	return (SUCCESS);
}

/*PING NON FONCTIONNEL ENCORE*/
int			MyMsg::PingCmd( MyServer *IRC_Server )
{
	std::string msg_sent;
	if (this->Params.empty())
	{
		msg_sent = ERR_NOORIGIN(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		msg_sent = "PONG " + this->_SentFrom->GetClientsNickname() + " :" + this->Params.at(0) + " \r\n";
		this->_SentFrom->SetClientsLastPing(std::time(0));
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	(void)IRC_Server;
	return (SUCCESS);
}

# include <unistd.h>
/*QUIT NON FONCTIONNEL ENCORE*/
int			MyMsg::QuitCmd( MyServer *IRC_Server )
{
	unsigned int			i;
	std::string				 msg_sent;

	msg_sent = "Quit: "; //message renvoyé selon le RFC modern.ircdocs.horse
	i = -1;
	if (this->Params.empty())
		msg_sent += this->_SentFrom->GetClientsNickname();  //message renvoyé selon si le Client n'a pas spécifié de raison
	if (this->Params.size() >= 1)
	{
		while (++i < this->Params.size())
			msg_sent = msg_sent + " " + this->Params[i]; // message renvoyé si le client a spécifié une raison
	}
	SendMsgBackWithPrefix(*this, msg_sent);
(void)IRC_Server;
	return (SUCCESS);
}

/*RENVOIT LE MESSAGE OF THE DAY -- VERIFIER LE > 4 DANS LE PREMIER IF*/
int			MyMsg::MotdCmd( void )
{
	std::string msg_sent;
	std::string file_tmp;
	std::vector<std::string> file_content;
	std::vector<std::string>::iterator it;

	if (this->Params.size() > 4) // S'il y'a trop de paramètres (verifier le nb de parametre sur les ordis de 42, moi j'en ai 4 bizarrement)
	{
		msg_sent = ERR_NOMOTD(*this, 1);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		std::ifstream motd_file("./srcs/motd.txt");
		if (!motd_file.good()) // Si le fichier n'existe pas
		{
			msg_sent = ERR_NOMOTD(*this, 2);
			SendMsgBackWithPrefix(*this, msg_sent);
		}
		else // Si le fichier existe
		{
			msg_sent = RPL_MOTDSTART(*this); // Code pour commencer le msg
			SendMsgBackWithPrefix(*this, msg_sent);
			while (getline(motd_file, file_tmp)) // j'écris dans la variable le contenu de motd.txt
				file_content.push_back(file_tmp);
			it = file_content.begin();
			while (it != file_content.end())
			{
				msg_sent = RPL_MOTD(*this, it);
				SendMsgBackWithPrefix(*this, msg_sent); //je le renvoi ligne par ligne au client
				it++;
			}
		}
		motd_file.close();
		msg_sent = RPL_ENDOFMOTD(*this); // je ferme le stream et je renvoi le code signifiant la fin du msg
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	return (SUCCESS);
}

int	MyMsg::PrivMsgCmd( MyServer *Irc_Server )
{
	std::string msg_sent;
	std::string	tmp;
	int			ret_find_first_of;
	size_t		ping_pos;

	ret_find_first_of = this->Params.at(0).find_first_of("!#+&");
	if (this->Params.empty())
	{
		msg_sent = ERR_NORECIPENT(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.size() == 1)
	{
		msg_sent = ERR_NOTEXTTOSEND(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (ret_find_first_of != 0 && Irc_Server->GetClientsThroughName(this->Params.at(0)) == NULL)
	{
		msg_sent = ERR_NOSUCHNICK(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (ret_find_first_of == 0 && Irc_Server->GetChannelsByName(this->Params.at(0)) == NULL)
	{
		msg_sent = ERR_NOSUCHNICK(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		ping_pos = this->_Message.find(':', this->_Prefix.size() + this->_Command.size() + this->Params.at(0).size());
		if (ping_pos != std::string::npos)
			tmp = this->_Message.substr(ping_pos);
		if (ret_find_first_of == 0)
		{
			std::cout << PURPLE << "Je suis dans PRIVMSG channel !" << NORMAL << std::endl;
			msg_sent = "PRIVMSG ";
			msg_sent = msg_sent + this->Params.at(0) + " " + tmp + "\r\n";
			std::cout << GREEN << "Channel name == " << Irc_Server->GetChannelsByName(this->Params.at(0)) << NORMAL << std::endl;
			Irc_Server->GetChannelsByName(this->Params.at(0))->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
			SendMsgBackWithPrefix(*this, msg_sent);
		}
		/*Ne pas oublier le cas avec les channels*/
		else
			RPL_PRIVMSG(this, tmp, 0); // RPL que j'ai inventé, ce RPL n'existe pas dans le RFC
	}
	return (SUCCESS);
}

int	MyMsg::NoticeCmd( MyServer *Irc_Server )
{
	std::string msg_sent;
	int			ret_find_first_of;
	std::string	tmp;
	size_t		ping_pos;


	ret_find_first_of = this->Params.at(0).find_first_of("!#+&");
	if (this->Params.size() < 2)
		return (FAILURE);
	else if (ret_find_first_of != 0 && Irc_Server->GetClientsThroughName(this->Params.at(0)) == NULL)
		return (FAILURE);
	else if (ret_find_first_of == 0 && Irc_Server->GetChannelsByName(this->Params.at(0)) == NULL)
		return (FAILURE);
	/*Ne pas oublier le if de la channels*/
	else
	{
		ping_pos = this->_Message.find(':', this->_Prefix.size() + this->_Command.size() + this->Params.at(0).size());
		if (ping_pos != std::string::npos)
		{
			tmp = this->_Message.substr(ping_pos);
			RPL_PRIVMSG(this, tmp, 1); // RPL que j'ai inventé, ce RPL n'existe pas dans le RFC
		}
	}
	return (SUCCESS);
}

int		MyMsg::VersionCmd( void )
{
	std::vector<std::string>::iterator it;
	
	it = this->Params.begin();
	if (this->Params.empty())
		SendMsgBackToClients(*this, RPL_VERSION(*this));
	else if (this->Params.size() == 1)
		SendMsgBackToClients(*this, ERR_NOSUCHSERVER(*this, it));
	// else
	// 	SendMsgBackToClients(*this, "Wrong params");
	return (SUCCESS);
}


int		MyMsg::JoinCmd( MyServer *IRC_Server )
{
	std::string 						msg_sent;
	char								*tmp;
	std::vector<std::string> 			channels;
	std::vector<std::string>::iterator	it;

	if (this->Params.empty())
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		tmp = strdup(this->Params.at(0).c_str());
		channels = IRC_Server->SplitByEndline(tmp, ",");
		it = channels.begin();
		free(tmp);
		while (it != channels.end())
		{
			if ((*it)[0] != '#' && (*it)[0] != '&' && (*it)[0] != '+' && (*it)[0] != '!')
				it->insert(it->begin(), '#');
			if (it->size() > 50) //CHANLIMIT
				it->resize(50);
			if (IRC_Server->GetChannelsByName(*it) == NULL)
				IRC_Server->CreateChannels(new Channels(this->_SentFrom, *it));
			IRC_Server->GetChannelsByName(*it)->AddClientsToChannelMemberList(this->_SentFrom);
			msg_sent = "JOIN" + *it;
			SendMsgBackWithPrefix(*this, msg_sent);
			IRC_Server->GetChannelsByName(*it)->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
			//Commande NAMES
			it++;
		}
	}
	
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
	this->MotdCmd();
	return (SUCCESS);
}
