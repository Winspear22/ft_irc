# include "num_replies.cpp"

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
		/*Pas sûr pour mode*/
		if (*it == "2")
			this->_SentFrom->SetClientsMode("w");
		else if (*it == "3")
			this->_SentFrom->SetClientsMode("i");
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
			ValidateClientsConnections(IRC_Server);
	}
	return (SUCCESS);
}