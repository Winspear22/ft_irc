# include "num_replies.cpp"

int		MyMsg::KickCmd( MyServer *IRC_Server )
{
	std::string msg_sent;

	if (this->Params.size() == 0)
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.size() >= 2)
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
				msg_sent = ERR_NOSUCHCHANNEL(*this, *it);
				SendMsgBackWithPrefix(*this, msg_sent); 
			}
			else if (this->_SentFrom->GetClientsMode().find('o') == std::string::npos)
			{
				msg_sent = ERR_CHANOPRIVSNEEDED2(*this, *it);
				SendMsgBackWithPrefix(*this, msg_sent);
			}
			else if (IRC_Server->GetChannelsByName(*it)->GetClientsInChannelMemberList(this->_SentFrom->GetClientsNickname()) == NULL)
			{
				msg_sent = ERR_NOTONCHANNEL(*this, *it);
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
						msg_sent = ERR_USERNOTINCHANNEL(*it2, *it);
						SendMsgBackWithPrefix(*this, msg_sent);
					}
					else
					{
						size_t ping_pos = this->_Message.find(':', this->GetPrefix().size() + this->GetCmd().size() + this->Params[0].size() + this->Params[1].size());
						std::string tmp = "";
						if (ping_pos != std::string::npos)
							tmp = this->_Message.substr(ping_pos);
						msg_sent = "KICK " + *it + " " + *it2 + " " + tmp;
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
	else
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	return (SUCCESS);
}