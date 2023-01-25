# include "num_replies.hpp"

int			MyMsg::ChanModeCmd( MyServer *IRC_Server )
{
	unsigned int i;
	int j;
	std::string mode;
	std::string msg_sent;
	i = 0;

	if (this->Params.size() == 1)
	{
		if (this->Params.at(0) == IRC_Server->GetChannelsByName(this->Params.at(0))->GetChannelName())
		{
			msg_sent = RPL_CHANNELMODEIS(IRC_Server->GetChannelsByName(this->Params.at(0)));
			SendMsgBackWithPrefix(*this, msg_sent);
		}
	}
	else if (this->Params.size() < 3)
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.at(2) != IRC_Server->GetClientsThroughName(this->Params.at(2))->GetClientsNickname())
	{
		msg_sent = ERR_USERNOTINCHANNEL(IRC_Server->GetClientsThroughName(this->Params.at(2))->GetClientsNickname(), IRC_Server->GetChannelsByName(this->Params.at(0))->GetChannelName());
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		mode = this->Params.at(1);
		if (IRC_Server->GetChannelsByName(this->Params.at(0))->GetChannelMode().empty())
			IRC_Server->GetChannelsByName(this->Params.at(0))->SetChannelMode(mode);
		while (i < mode.size())
		{
			if (mode[i] == '-')
			{
				j = j + 1;
				while (isalpha(mode[j]))
				{
					if (mode[j] == 'i')
					{
						IRC_Server->GetChannelsByName(this->Params.at(0))->DeleteChannelsMode(mode[j]);
						IRC_Server->GetChannelsByName(this->Params.at(0))->DeleteClientsFromAllowedToInviteList(IRC_Server->GetClientsThroughName(this->Params.at(2)));
					}
					else
					{
						std::string mode2;
						mode2 += mode[j];
						msg_sent = ERR_UNKNOWNMODE(*this, mode2, IRC_Server->GetChannelsByName(this->Params.at(0))->GetChannelName());
						SendMsgBackWithPrefix(*this, msg_sent);

					}
					j++;
				}
			}
			if (mode[i] == '+')
			{
				j = j + 1;
				while (isalpha(mode[j]))
				{
					if (mode[j] == 'i')
					{
						std::map<Clients *, int>::iterator it;
						IRC_Server->GetChannelsByName(this->Params.at(0))->AddChannelsMode(mode[j]);
						IRC_Server->GetChannelsByName(this->Params.at(0))->AddClientsToAllowedToInviteList(IRC_Server->GetClientsThroughName(this->Params.at(2)));

						it = IRC_Server->GetChannelsByName(this->Params.at(0))->GetAllowedToInviteList().begin();
						while (it != IRC_Server->GetChannelsByName(this->Params.at(0))->GetAllowedToInviteList().end())
						{
							msg_sent = "346 " + IRC_Server->GetChannelsByName(this->Params.at(0))->GetChannelName() + " ";
							msg_sent += it->first->GetClientsNickname();
							SendMsgBackWithPrefix(*this, msg_sent);
							it++;
						}
						msg_sent = "347 " + IRC_Server->GetChannelsByName(this->Params.at(0))->GetChannelName() + " :End of channel invite list";
						SendMsgBackWithPrefix(*this, msg_sent);
					}
					else
					{
						std::string mode2;
						mode2 += mode[j];
						msg_sent = ERR_UNKNOWNMODE(*this, mode2, IRC_Server->GetChannelsByName(this->Params.at(0))->GetChannelName());
						SendMsgBackWithPrefix(*this, msg_sent);
												IRC_Server->GetChannelsByName(this->Params.at(0))->DeleteClientsFromAllowedToInviteList(IRC_Server->GetClientsThroughName(this->Params.at(0)));

					}
					j++;
				}
			}
			i++;
		}	
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
	else if (this->Params.at(0) != this->_SentFrom->GetClientsNickname())
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