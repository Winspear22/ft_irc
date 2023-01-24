# include "num_replies.hpp"

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
	else if (IRC_Server->isUnavailableNickname(*it))
	{
		msg_sent = ERR_UNAVAILRESOURCE(*it);
		SendMsgBackToClients(*this, msg_sent);
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
		msg_sent = msg_sent + *it; // Ne pas oublier le \n pour signifier la fin du retour de cmd.
		SendMsgBackWithPrefix(*this, msg_sent);
		this->_SentFrom->SetClientsConnectionNickCmd(YES);
		if (this->_SentFrom->GetClientsConnectionUserCmd() == YES && this->_SentFrom->GetClientsConnectionNickCmd() == YES && this->_SentFrom->GetClientsConnectionAuthorisation() == YES \
		&& this->_SentFrom->GetClientsConnectionPermission() == NO)
			ValidateClientsConnections(IRC_Server);
		return (SUCCESS);
	}
	else if (IRC_Server->GetClientsThroughName(*it) == NULL && this->NickFormatCheck(nick_format_check) == SUCCESS && this->_SentFrom->_Nickname.size() != 0)
	{
		msg_sent = "NICK ";
		msg_sent = msg_sent + *it;
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
