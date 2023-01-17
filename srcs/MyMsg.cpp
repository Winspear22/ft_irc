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
	std::string tmp = "";

	if (this->_SentFrom->_Nickname.size())
	{
		tmp = ":" + this->_SentFrom->_Nickname;
		if (this->_SentFrom->_Nickname.size())
			tmp = tmp + "!" + this->_SentFrom->_Nickname;
		if (this->_SentFrom->_Nickname.size())
			tmp = tmp + "@" + this->_SentFrom->_Hostname;
		else
			tmp = tmp + "@" + this->_SentFrom->_HostAdress;
	}
	else
		tmp = ":" + this->_SentFrom->_ServerName;
	this->Prefix = tmp;
	return (this->Prefix);
}

std::string MyMsg::GetCmd( void )
{
	return (this->Command);
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
	std::vector<std::string>::iterator 	it;
	int									i;
	unsigned long						j;
	
	it = cmd_list.begin();
	i = 0;
	j = 0;
	if (cmd->empty())
		return (FAILURE);
	if (isdigit(cmd->at(0)) != SUCCESS && cmd->size() == 3)
	{
		while (i < 3)
		{
			if (isdigit(cmd->at(i)) != SUCCESS)
				return (FAILURE);
			i++;
		}
		return (SUCCESS);
	}
	while (j < cmd->size())
	{
		if (isalpha(cmd->at(i)) == SUCCESS)
			return (FAILURE);
		j++;
	}
	(void)cmd_list;
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
		this->_SentFrom->SetClientsConnectionAuthorisation(YES);
		SendMsgBackToClients(*this, msg_sent);
		this->_SentFrom->SetClientsConnectionAuthorisation(NO);
	}
	else if (*it != IRC_Server->GetPassword())
	{
		msg_sent = ERR_PASSWDMISMATCH();
		this->_SentFrom->SetClientsConnectionAuthorisation(YES);
		SendMsgBackToClients(*this, msg_sent);
		this->_SentFrom->SetClientsConnectionAuthorisation(NO);
	}	
	else if (!this->Params.empty() && this->_SentFrom->GetClientsConnectionAuthorisation() == YES)
	{
		msg_sent = ERR_ALREADYREGISTRED(*this);
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
//int	MyMsg::NickFormatCheck( std::string nickcheck )
{
	unsigned int 	i;
	int 			j;
	int				not_a_special_character;
	char 			special_characters[9] = {'-', '_', '[', ']', '{', '}', '\\', '`', '|'};
	
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
			if (nickcheck->at(i) == special_characters[j])
			{
				not_a_special_character = 1;
				break ;
			}
			else
				not_a_special_character = 0;
		}
		if (not_a_special_character == 0 && !isalnum(nickcheck->at(i)))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

/*LA COMMANDE NICK QUI INITIALISE LE NICNAME*/
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
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.size() > 1) // Est-ce que plrs params ont été mis --> NICK popo lolo
	{
		msg_sent = ERR_NONICKNAMEGIVEN();
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (it->size() > 9 || it->size() == 0) // La taille du NICK est plus grand que 9 ou vide --> NICK lolololololo OU NICK ""
	{
		msg_sent = ERR_ERRONEUSNICKNAME(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->NickFormatCheck(nick_format_check) == FAILURE) // Format illégal du NICK --> NICK 1popo 
	{
		msg_sent = ERR_ERRONEUSNICKNAME(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (IRC_Server->GetClientsThroughName(*it) != NULL) // Verifier si le pseudo existe ou non --> NICK user42 NICK user42
	{
		msg_sent = ERR_NICKNAMEINUSE(*this, it);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (IRC_Server->GetClientsThroughName(*it) == NULL  && this->NickFormatCheck(nick_format_check) == SUCCESS && this->_SentFrom->_Nickname.size() == 0) // Tout va bien, alors je peux remplir le Nickname
	{
		this->_SentFrom->SetClientsNickname(*it);
		msg_sent = "NICK ";
		msg_sent = msg_sent + *it + "\r\n"; // Ne pas oublier le \n pour signifier la fin du retour de cmd.
		SendMsgBackWithPrefix(*this, msg_sent);
		this->_SentFrom->SetClientsConnectionNickCmd(YES);
		if (this->_SentFrom->GetClientsConnectionUserCmd() == YES && this->_SentFrom->GetClientsConnectionNickCmd() == YES && this->_SentFrom->GetClientsConnectionAuthorisation() == YES \
		&& this->_SentFrom->GetClientsConnectionPermission() == NO)
			ValidateClientsConnections();
		return (SUCCESS);
	}
	else if (IRC_Server->GetClientsThroughName(*it) == NULL && this->NickFormatCheck(nick_format_check) == SUCCESS && this->_SentFrom->_Nickname.size() != 0)
	{
		msg_sent = "NICK ";
		msg_sent = msg_sent + *it + "\r\n";
		channels_it = IRC_Server->channels_list.begin();
		while (channels_it != IRC_Server->channels_list.end())
		{
			if (channels_it->first->_CreatedBy != NULL)
			{	
				if (channels_it->first->_CreatedBy != NULL)
					channels_it->first->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
			}
			channels_it++;
		}
		SendMsgBackWithPrefix(*this, msg_sent);
		this->_SentFrom->SetClientsNickname(*it);

		return (SUCCESS);
	}
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
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->_SentFrom->GetClientsUsername().empty() == false)
	{
		msg_sent = ERR_ALREADYREGISTRED(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.size() >= 4) // > 4 car le realname peut contenir des  espaces
	{
		while (it != this->Params.end())
		{
			if (it->empty() == true)
			{
				msg_sent = ERR_NEEDMOREPARAMS(*this);
				SendMsgBackWithPrefix(*this, msg_sent);
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
				realname = this->_SentFrom->_Nickname;
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
		if (this->_SentFrom->GetClientsConnectionUserCmd() == YES && this->_SentFrom->GetClientsConnectionNickCmd() == YES && this->_SentFrom->GetClientsConnectionAuthorisation() == YES \
		&& this->_SentFrom->GetClientsConnectionPermission() == NO)
			ValidateClientsConnections();
	}
	return (SUCCESS);
}




int			MyMsg::ModeCmd( MyServer *IRC_Server )
{
	std::string msg_sent;
	int ret_find_first_of;
	std::string mode;
	unsigned int i;
	int j;

	i = 0;
	ret_find_first_of = this->Params.at(0).find_first_of("!&+#");
	if (this->Params.empty())
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (ret_find_first_of == 0)
		return (FAILURE);
	else if (this->Params.at(0) != this->_SentFrom->_Nickname) //+':' mais je sais pas pk sa marche pas sans
	{
		msg_sent = ERR_USERSDONTMATCH(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.size() == 1)
	{
		msg_sent = RPL_UMODEIS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		mode = this->Params.at(1);
		static int start = 0;
		if (start == 0)
		{
			this->_SentFrom->SetClientsMode(mode);
			start++;
		}
		while (i < mode.size())
		{
			if (mode[i] == '-')
			{
				j = i + 1;
				while (isalpha(mode[j]))
				{
					if (mode[j] == 'i' || mode[j] == 'w' || mode[j] == 'o')
						this->_SentFrom->DeleteClientsMode(mode[j]);
					else if (mode[j] != 'O')
					{
						msg_sent = ERR_UMODEUNKNOWNFLAG(*this);
						SendMsgBackWithPrefix(*this, msg_sent);
					}
					j++;
				}
			}
			if (mode[i] == '+')
			{
				j = i + 1;
				while (isalpha(mode[j]))
				{
					if (mode[j] == 'i' || mode[j] == 'w')
						this->_SentFrom->AddClientsMode(mode[j]);
					else if (mode[j] != 'O' || mode[j] == 'o')
					{
						msg_sent = ERR_UMODEUNKNOWNFLAG(*this);
						SendMsgBackWithPrefix(*this, msg_sent);
					}
					j++;
				}
			}
			i++;
		}
		msg_sent = RPL_UMODEIS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}

	(void)IRC_Server;
	return (SUCCESS);
}

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
		msg_sent = "PONG " + this->_SentFrom->_Nickname + " :" + this->Params.at(0);
		this->_SentFrom->SetClientsLastPing(std::time(0));
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	(void)IRC_Server;
	return (SUCCESS);
}

int			MyMsg::QuitCmd( MyServer *IRC_Server )
{
	unsigned int								i;
	std::string				 					msg_sent;
	std::map<Channels *, std::string>::iterator it;

	msg_sent = "QUIT"; //message renvoyé selon le RFC modern.ircdocs.horse
	i = -1;
	if (this->Params.empty())
		msg_sent = msg_sent +  " " + ":Client quit";  //message renvoyé selon si le Client n'a pas spécifié de raison
	if (this->Params.size() >= 1)
	{
		while (++i < this->Params.size())
			msg_sent = msg_sent + " " + this->Params[i]; // message renvoyé si le client a spécifié une raison
	}
	it = IRC_Server->channels_list.begin();
	while (it != IRC_Server->channels_list.end())
	{
		if (it->first->GetClientsInChannelMemberList(this->_SentFrom->GetClientsNickname()) != NULL)
		{
			std::cout << "User supprimé du channel" << std::endl;
		//	if (it->first->GetChannelCreator()->GetClientsNickname() == this->_SentFrom->GetClientsNickname())
		//		it->first->SetHasAChannelCreator(NO);
			it->first->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
			it->first->DeleteClientsToChannelMemberList(this->_SentFrom);
		}
		it++;
	}
	
	SendMsgBackWithPrefix(*this, msg_sent);
	if (IRC_Server->GetClientsThroughSocketFd(this->_SentFrom->GetClientsFd()) != NULL && this->_SentFrom != NULL)
	{
		IRC_Server->_clients_list.erase(IRC_Server->GetClientsThroughSocketFd(this->_SentFrom->GetClientsFd()));
		delete this->_SentFrom;
	}
	return (SUCCESS);
}

/*RENVOIT LE MESSAGE OF THE DAY -- VERIFIER LE > 4 DANS LE PREMIER IF*/
int			MyMsg::MotdCmd( void )
{
	std::string msg_sent;
	std::string file_tmp;
	std::vector<std::string> file_content;
	std::vector<std::string>::iterator it;

	/*if (this->Params.size() > 1) // S'il y'a trop de paramètres (verifier le nb de parametre sur les ordis de 42, moi j'en ai 4 bizarrement)
	{
		it = this->Params.begin();
		while (it != this->Params.end())
		{
			std::cout << "Params == " << *it << std::endl;
			it++;
		}
		msg_sent = ERR_NOMOTD(*this, 1);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{*/
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
	//}
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
		ping_pos = this->_Message.find(':', this->Prefix.size() + this->Command.size() + this->Params.at(0).size());
		if (ping_pos != std::string::npos)
			tmp = this->_Message.substr(ping_pos);
		if (ret_find_first_of == 0)
		{
			msg_sent = "PRIVMSG ";
			msg_sent = msg_sent + this->Params.at(0) + " " + tmp;// + "\r\n";
			Irc_Server->GetChannelsByName(this->Params.at(0))->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
		}
		else
			RPL_PRIVMSG(this, tmp, 0, Irc_Server); // RPL que j'ai inventé, ce RPL n'existe pas dans le RFC
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
	else
	{
		ping_pos = this->_Message.find(':', this->Prefix.size() + this->Command.size() + this->Params.at(0).size());
		if (ping_pos != std::string::npos)
		{
			tmp = this->_Message.substr(ping_pos);
			RPL_PRIVMSG(this, tmp, 1, Irc_Server); // RPL que j'ai inventé, ce RPL n'existe pas dans le RFC
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
	return (SUCCESS);
}


int			MyMsg::IsTheUserVisible( Channels *channel, Clients *sender )
{
	std::map<Clients *, int>::iterator it;

	if (channel == NULL || sender == NULL)
		return false;
	it = channel->_MemberOfTheChannelList.begin();
	while (it != channel->_MemberOfTheChannelList.end())
	{
		if (it->first->GetClientsMode().find('i') == std::string::npos || it->first == sender)
			return (SUCCESS);
		it++;
	}
	return (FAILURE);
}

int			MyMsg::NamesCmd( MyServer *IRC_Server, MyMsg &msg )
{
	std::string str;

	if (msg.Params.size() == 0)
	{
		std::map<Channels *, std::string> chans = IRC_Server->channels_list;
		std::map<Channels *, std::string>::iterator it = chans.begin();
		while (it != chans.end())
		{
			std::map<Clients *, int> members = IRC_Server->GetChannelsByName(it->second)->_MemberOfTheChannelList;
			std::map<Clients *, int>::iterator it2 = members.begin();
			str = "353";
			str += " " + msg._SentFrom->_Nickname + " =" + it->second + " :";
			while (it2 != members.end())
			{
				str += it2->first->_Nickname + " ";
				it2++;
			}
			SendMsgBackWithPrefix(msg, str);
			str = "366";
			str += " " + msg._SentFrom->_Nickname + " ";
			str += it->second + ":End of /NAMES list";
			SendMsgBackWithPrefix(msg, str);
			it++;
		}
	}
	else
	{
		char *tmp = strdup(msg.Params[0].c_str());
		std::vector<std::string> channels = IRC_Server->SplitByEndline(tmp, ","); //changer ici vector->list
		std::vector<std::string>::iterator it = channels.begin();
		free(tmp);
		while (it != channels.end())
		{
			if (IRC_Server->GetChannelsByName(*it) == NULL)
				it++;
			else
			{
				if (msg.IsTheUserVisible(IRC_Server->GetChannelsByName(*it), msg._SentFrom) == SUCCESS)
				{
					std::map<Clients *, int> members = IRC_Server->GetChannelsByName(*it)->_MemberOfTheChannelList;
					std::map<Clients *, int>::iterator it2 = members.begin();
					str = "353";
					str += " " + msg._SentFrom->_Nickname + " = " + *it + " :";
					while (it2 != members.end())
					{
						if (it2->first->GetClientsMode().find('i') == std::string::npos || it2->first == msg._SentFrom || IRC_Server->GetChannelsByName(*it)->GetClientsInChannelMemberList(msg._SentFrom->_Nickname))
						{
							if (IRC_Server->GetChannelsByName(*it)->GetChannelCreator() != NULL)
							{
								if (it2->first == IRC_Server->GetChannelsByName(*it)->GetChannelCreator()) //changer ici + fd
									str += "@";
								str += it2->first->_Nickname + " ";
							}
						}
						it2++;
					}			
				}
				if (IRC_Server->GetChannelsByName(*it) != NULL)
				{
					SendMsgBackWithPrefix(msg, str);
					str = "366";
					str += " " + msg._SentFrom->_Nickname + " ";
					str += *it + " :End of /NAMES list";
					SendMsgBackWithPrefix(msg, str);
				}
				it++;
			}
		}
	}
	return (SUCCESS);
}

int		MyMsg::JoinCmd( MyServer *IRC_Server )
{
	std::string 						msg_sent;
	char								*tmp;
	std::vector<std::string> 			channels;
	std::vector<std::string>::iterator	it;
	Channels							*chan;

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
			{
				chan = IRC_Server->CreateChannels(*it, this->_SentFrom);//new Channels(this->_SentFrom, *it);
				chan->AddClientsToChannelMemberList(this->_SentFrom);
				msg_sent = "JOIN " + *it;
				SendMsgBackWithPrefix(*this, msg_sent);
				chan->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
				MyMsg UseOfNamesCmd(this->_SentFrom, "NAMES " + *it);
				UseOfNamesCmd.parse_msg();
				NamesCmd(IRC_Server, UseOfNamesCmd);
			}
			else
			{
				IRC_Server->GetChannelsByName(*it)->AddClientsToChannelMemberList(this->_SentFrom);
				msg_sent = "JOIN " + *it;
				SendMsgBackWithPrefix(*this, msg_sent);
				IRC_Server->GetChannelsByName(*it)->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
				MyMsg UseOfNamesCmd(this->_SentFrom, "NAMES " + *it);
				UseOfNamesCmd.parse_msg();
				NamesCmd(IRC_Server, UseOfNamesCmd);
			}
			it++;
		}
	}
	
	return (SUCCESS);
}

#include <sstream>

int		MyMsg::ListCmd( MyServer *IRC_Server )
{
	std::string 								msg_sent;
	char										*tmp;
	std::vector<std::string> 					channels;
	std::map<Channels*, std::string>::iterator	it1;
	std::vector<std::string>::iterator 			it2;
	Channels									*chan;
	std::string									name_chan;
	std::string									nb_members;
	size_t										nb_memberss;

	if (this->Params.empty())
	{
		it1 = IRC_Server->channels_list.begin();
		while (it1 != IRC_Server->channels_list.end())
		{
			chan = it1->first;
			name_chan = chan->GetChannelName();
			nb_memberss = chan->GetAllClientsInChannelMemberList().size();
			std::ostringstream convert;
			convert << nb_memberss;
			nb_members = convert.str();
			msg_sent = RPL_LIST(*this, name_chan, nb_members);
			SendMsgBackToClients(*this, msg_sent);
			it1++;
		}
	}
	else
	{
		tmp = strdup(this->Params.at(0).c_str());
		channels = IRC_Server->SplitByEndline(tmp, ",");
		it2 = channels.begin();
		free(tmp);
		while (it2 != channels.end())
		{
			if (IRC_Server->GetChannelsByName(*it2) != NULL)
			{
				name_chan = *it2;
				nb_memberss = IRC_Server->GetChannelsByName(*it2)->GetAllClientsInChannelMemberList().size();
				std::ostringstream convert;
				convert << nb_memberss;
				nb_members = convert.str();
				msg_sent = RPL_LIST(*this, name_chan, nb_members);
				SendMsgBackToClients(*this, msg_sent);
			}
			it2++;
		}
	}
	msg_sent = RPL_LISTEND(*this);
	SendMsgBackWithPrefix(*this, msg_sent);
	return (SUCCESS);
}

std::string get_time_compilation()
{
    int         fd;
    struct stat file;
    std::string time;

    if ((fd = open("./ircserv", O_RDONLY)) > 0)
    {
        fstat(fd, &file);
        time = ctime(&file.st_mtime); // st_mtime = time of last modification
    }
	close(fd);
    return (time);
}

int		MyMsg::InfoCmd( void )
{
	std::string msg_sent;
	std::string file_tmp;
	std::vector<std::string> file_content;
	std::vector<std::string>::iterator it1;
	std::vector<std::string>::iterator it2;
	size_t			pos;
	std::string		str;
	std::string	time_compilation = get_time_compilation();

	it1 = this->Params.begin();
	if (this->Params.size() >= 1) // S'il y'a trop de paramètres (verifier le nb de parametre sur les ordis de 42, moi j'en ai 4 bizarrement)
	{
		msg_sent = ERR_NOSUCHSERVER(*this, it1);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		std::ifstream info_file("./srcs/info.txt");
		if (!info_file.good()) // Si le fichier n'existe pas
			; // ne rien faire
		else // Si le fichier existe
		{
			while (getline(info_file, file_tmp)) // j'écris dans la variable le contenu de motd.txt
			{
				if (((pos = file_tmp.find("$date")) != std::string::npos))
				{
					str = file_tmp.substr(0, pos) + time_compilation + file_tmp.substr(pos + strlen("$date"));
					file_tmp = str;
				}
				file_content.push_back(file_tmp);
			}
			it2 = file_content.begin();
			while (it2 != file_content.end())
			{
				msg_sent = RPL_MOTD(*this, it2);
				SendMsgBackWithPrefix(*this, msg_sent); //je le renvoi ligne par ligne au client
				it2++;
			}
		}
		info_file.close();
		msg_sent = RPL_ENDOFINFO(*this); // je ferme le stream et je renvoi le code signifiant la fin du msg
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	return (SUCCESS);
}

int		MyMsg::KickCmd( MyServer *IRC_Server )
{
	std::string msg_sent;

	if (this->Params.size() == 0)
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		char *tmp = strdup(this->Params[0].c_str());
		std::vector<std::string> channels = IRC_Server->SplitByEndline(tmp, ",");
		free(tmp);
		tmp = strdup(this->Params[1].c_str());
		std::vector<std::string> users = IRC_Server->SplitByEndline(tmp, ",");
		free(tmp);
		std::vector<std::string>::iterator it;

		it = channels.begin();
		while (it != channels.end())
		{
			if (IRC_Server->GetChannelsByName(*it) == NULL)
			{
				msg_sent = "403";
				msg_sent += " " + this->_SentFrom->GetClientsNickname();
				msg_sent += " ";
				msg_sent += *it;
				msg_sent += " :No such channel.";
				//msg_sent = ERR_NOSUCHCHANNEL(*this, *it);
				SendMsgBackWithPrefix(*this, msg_sent); 
			}
			else if (this->_SentFrom->GetClientsMode().find('o') == std::string::npos)
			{
				msg_sent = "482";
				msg_sent += " " + this->_SentFrom->GetClientsNickname();
				msg_sent += " " + *it;
				msg_sent += " :You're not channel operator";
				SendMsgBackWithPrefix(*this, msg_sent);
			}
			else
			{
				std::vector<std::string>::iterator it2;
				it2 = users.begin();
				while (it2 != users.end())
				{
					if (IRC_Server->GetClientsThroughName(*it2) == NULL || IRC_Server->GetChannelsByName(*it)->GetClientsInChannelMemberList(*it2) == NULL)
					{
						msg_sent = "441";
						msg_sent += " " + this->_SentFrom->GetClientsNickname();
						msg_sent += " " + *it2 + " " + *it;
						msg_sent += " :They aren't on that channel.";
						SendMsgBackWithPrefix(*this, msg_sent);
					}
					else
					{
						size_t start = this->_Message.find(':', this->GetPrefix().size() + this->GetCmd().size() + this->Params[0].size() + this->Params[1].size());
						std::string text = "";
						if (start != std::string::npos)
							text = this->_Message.substr(start);
						msg_sent = "KICK " + *it + " " + *it2 + " " + text;
						IRC_Server->GetChannelsByName(*it)->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
						SendMsgBackWithPrefix(*this, msg_sent);
						IRC_Server->GetChannelsByName(*it)->DeleteClientsToChannelMemberList(IRC_Server->GetClientsThroughName(*it2));
					}		
					it2++;
				}
			}
			it++;
		}
	}
	return (SUCCESS);
}

int		MyMsg::PartCmd( MyServer *IRC_Server )
{
	std::string 								msg_sent;
	std::string									stmp;
	char										*tmp;
	std::vector<std::string> 					channels;
	std::vector<std::string>::iterator 			it;

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
			if (IRC_Server->GetChannelsByName(*it) == NULL)
			{
				stmp = *it;
				msg_sent = ERR_NOSUCHCHANNEL(*this, stmp);
				SendMsgBackToClients(*this, msg_sent);
			}
			else if (IRC_Server->GetChannelsByName(*it)->GetClientsInChannelMemberList(this->GetClients()->GetClientsNickname()) == NULL)
			{
				msg_sent = ERR_NOTONCHANNEL(*this, it);
				SendMsgBackToClients(*this, msg_sent);
			}
			else
			{
				IRC_Server->GetChannelsByName(*it)->DeleteClientsToChannelMemberList(IRC_Server->GetChannelsByName(*it)->GetClientsInChannelMemberList(this->GetClients()->GetClientsNickname()));
				if (this->Params.size() > 1)
				{
					msg_sent = "PART " + *it + " ";
					size_t i = 1;
					while (i < this->Params.size())
					{
						msg_sent += this->Params.at(i);
						if (i < this->Params.size() - 1)
							msg_sent += " ";
						i++;
					}
					msg_sent += "\r\n";
				}
				else
					msg_sent = "PART " + *it + " " + this->GetClients()->GetClientsNickname() + "\r\n";
				IRC_Server->GetChannelsByName(*it)->SendMsgToAllInChannels(this,msg_sent,this->_SentFrom);
				SendMsgBackWithPrefix(*this, msg_sent);
				if (IRC_Server->GetChannelsByName(*it)->_MemberOfTheChannelList.empty())
					std::cout << "J'ai supprimé le channel : " << *it << std::endl;

			}
			it++;
		}
	}
	return (SUCCESS);
}

int		MyMsg::WallopsCmd( MyServer *IRC_Server )
{
	std::string msg_sent;

	std::cout << GREEN << "Je suis dans WALLOPS normal" << std::endl;

	if (this->Params.empty())
	{
		std::cout << GREEN << "Je suis dans WALLOPS error" << std::endl;
		msg_sent = "461";
		msg_sent += " " + this->_SentFrom->GetClientsNickname() + " " + ":Not enough parameters";
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->_SentFrom->GetClientsMode().find('o') == std::string::npos)
	{
		msg_sent = "481";
		msg_sent += " " + this->_SentFrom->GetClientsNickname() + " " + ":Permission denied- You're not an IRC operator";
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->_SentFrom->GetClientsMode().find('o') != std::string::npos)
	{
		std::cout << GREEN << "Je suis dans WALLOPS" << std::endl;
		size_t start = this->_Message.find(':', this->Prefix.size() + this->Command.size());
		std::string text = "";
		if (start != std::string::npos)
			text = this->_Message.substr(start);
		msg_sent = this->Prefix + " WALLOPS " + text;
		std::map<Clients*, int>::iterator it;
		it = IRC_Server->_clients_list.begin();
		while (it != IRC_Server->_clients_list.end())
		{
			if (it->first->GetClientsMode().find('w') != std::string::npos)
				SendMsgBackToClients(*this, msg_sent);
			it++;
		}
	}
	/*PAS DE ERR_NOPRIVS - CAR TOUS LES OPER ONT LES MEMES DROITS*/
	return (SUCCESS);
}

int		MyMsg::OperCmd( MyServer *IRC_Server )
{
	std::string msg_sent;
	(void)IRC_Server;

	if (this->Params.at(0) != ID_OF_OPERATORS || this->Params.at(1) != PASSWORD_OF_OPERATORS)
	{
		msg_sent = "464";
		msg_sent += " " + this->_SentFrom->GetClientsNickname() + " " + ":Password incorrect";
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.size() < 2)
	{
		msg_sent = "461";
		msg_sent += " " + this->_SentFrom->GetClientsNickname() + " " + ":Not enough parameters";
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		msg_sent = "381";
		msg_sent += " " + this->_SentFrom->GetClientsNickname() + " " + ":You are now an IRC operator";
		SendMsgBackWithPrefix(*this, msg_sent);
		this->_SentFrom->AddClientsMode('o');
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

	intro_new_nick = "\033[1;35mIntroducing new nick \033[1;37m" + this->_SentFrom->_Nickname + "\r\n";
	SendMsgBackToClients(*this, intro_new_nick);
	this->MotdCmd();
	return (SUCCESS);
}



/*int			MyMsg::NamesCmd( MyServer *IRC_Server )
{
	std::string str;

	if (this->Params.size() == 0)
	{
		std::map<Channels *, std::string> chans = IRC_Server->channels_list;
		std::map<Channels *, std::string>::iterator it = chans.begin();
		while (it != chans.end())
		{
			std::map<Clients *, int> members = IRC_Server->GetChannelsByName(it->second)->GetAllClientsInChannelMemberList();
			std::map<Clients *, int>::iterator it2 = members.begin();
			str = "353";
			str += " " + this->_SentFrom->_Nickname + " =" + it->second + " :";
			while (it2 != members.end())
			{
				str += it2->first->_Nickname + " ";
				it2++;
			}
			SendMsgBackWithPrefix(*this, str);
			str = "366";
			str += " " + this->_SentFrom->_Nickname + " ";
			str += it->second + ":End of /NAMES list";
			SendMsgBackWithPrefix(*this, str);
			it++;
		}
	}
	else
	{
		char *tmp = strdup(this->Params[0].c_str());
		std::vector<std::string> channels = IRC_Server->SplitByEndline(tmp, ","); //changer ici vector->list
		std::vector<std::string>::iterator it = channels.begin();
		free(tmp);
		while (it != channels.end())
		{
			if (this->IsTheUserVisible(IRC_Server->GetChannelsByName(*it), this->_SentFrom) == SUCCESS)
			{
				std::map<Clients *, int> members = IRC_Server->GetChannelsByName(*it)->GetAllClientsInChannelMemberList();
				std::map<Clients *, int>::iterator it2 = members.begin();
				str = "353";
				str += " " + this->_SentFrom->_Nickname + " = " + *it + " :";
				while (it2 != members.end())
				{
					if (it2->first->GetClientsMode().find('i') == std::string::npos || it2->first == this->_SentFrom || IRC_Server->GetChannelsByName(*it)->GetClientsInChannelMemberList(this->_SentFrom->_Nickname))
					{
						if (it2->second == IRC_Server->GetChannelsByName(*it)->GetChannelCreator()->GetClientsFd()) //changer ici + fd
							str += "@";
						str += it2->first->_Nickname + " ";
					}
					it2++;
				}			
			}
			if (IRC_Server->GetChannelsByName(*it) != NULL)
			{
				SendMsgBackWithPrefix(*this, str);
				str = "366";
				str += " " + this->_SentFrom->_Nickname + " ";
				str += *it + " :End of /NAMES list";
				SendMsgBackWithPrefix(*this, str);
			}
		}
	}
	return (SUCCESS);
}*/


bool MyMsg::Check_command(std::string str)
{
	if (str.size() == 0)
		return false;
	if (str.size() == 3 && isdigit(str[0]) == true)
	{
		for(int i = 0; i < 3; i++)
		{
			if (isdigit(str[i]) == false)
				return false;
		}
		return true;
	}
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isalpha(str[i]) == 0)
			return false;
	}
	return true;
}


void MyMsg::SetParams2(std::vector<std::string> params)
{
	this->Params = params;
}

bool	MyMsg::parse_msg(void)
{
	const char *delim = " ";
	char*	tmp = strdup(this->_Message.c_str());

	std::vector<std::string> 			tab_parse;
	std::vector<std::string>::iterator	str;

	char *token = strtok(tmp, delim);
	while(token != NULL)
	{
		tab_parse.push_back(std::string(token));
		token = strtok(NULL, delim);
	}
	free(tmp);

	if (tab_parse.size() < 1)
		return false;

	str = tab_parse.begin();
	if ((*str)[0] == ':')
	{
		this->Prefix = *str;
		++str;
	}
	if (Check_command(*str))
		this->Command = *str;
	else
	{
		std::cout << "Invalid command format." << std::endl;
		return false;
	}
	++str;
	if (str != tab_parse.end())
	{
		tab_parse.erase(tab_parse.begin(), str);
		this->SetParams2(tab_parse);
	}
	return true;
}