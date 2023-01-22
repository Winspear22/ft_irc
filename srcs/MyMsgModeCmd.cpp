# include "num_replies.cpp"

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
		if (this->_SentFrom->GetClientsMode().empty())
			this->_SentFrom->SetClientsMode(mode);
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