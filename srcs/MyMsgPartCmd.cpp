# include "num_replies.hpp"

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
				this->GetClients()->SetChanLim(this->GetClients()->GetChanLim() - 1);
				SendMsgBackWithPrefix(*this, msg_sent);
			}
			it++;
		}
	}
	return (SUCCESS);
}