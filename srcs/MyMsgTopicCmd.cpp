# include "num_replies.hpp"

int		MyMsg::TopicCmd( MyServer *IRC_Server )
{
	std::string 								msg_sent;
	std::map<Channels*, std::string>::iterator	it1;
	std::map<Clients*, int>::iterator it;

	if (this->Params.empty())
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	it1 = IRC_Server->channels_list.begin();
	if (this->Params.size() == 1)
	{
		while (it1 != IRC_Server->channels_list.end())
		{
			if (this->Params[0] == it1->second)
			{
				if (it1->first->GetClientsInChannelMemberList(this->_SentFrom->GetClientsNickname()) == NULL)
				{
					msg_sent = ERR_NOTONCHANNEL(it1);
					SendMsgBackWithPrefix(*this, msg_sent);
				} 
				else
				{
					if (it1->first->GetChannelstopic().empty())
					{
						msg_sent = RPL_NOTOPIC();
						SendMsgBackWithPrefix(*this, msg_sent);
					}
					else
					{
						msg_sent = RPL_TOPIC(*this, it1->second, it1->first->GetChannelstopic());
						SendMsgBackWithPrefix(*this, msg_sent);
					}
				}
			}
			it1++;
		}
	}
	else if (this->Params.size() >= 2)
	{
		while (it1 != IRC_Server->channels_list.end())
		{
			it = it1->first->GetAllClientsInChannelMemberList().begin();

			if (it1->second == this->Params[0])
			{
				if (this->_SentFrom->GetClientsMode().find('o') != std::string::npos || this->_SentFrom->GetClientsMode().find('O') != std::string::npos)
				{
					std::string 	tmp;
					size_t			ping_pos;

					tmp = "";
					ping_pos = this->_SentFrom->GetClientsMessage().find(':', this->Prefix.size() + this->Command.size() + this->Params.at(0).size());
					if (ping_pos != std::string::npos)
						tmp = this->_SentFrom->GetClientsMessage().substr(ping_pos + 1);
					it1->first->SetChannelstopic(tmp);
					msg_sent = RPL_TOPIC(*this, it1->second, it1->first->GetChannelstopic());
					std::string ret = "TOPIC " + it1->second + " :" + it1->first->GetChannelstopic();
					it1->first->SendMsgToAllInChannelsForTopic(this, msg_sent, this->_SentFrom);
				}
				else
				{
					msg_sent = ERR_CHANOPRIVSNEEDED(*this, it1);
					SendMsgBackWithPrefix(*this, msg_sent);
				}
			}
			it1++;
		}
	}

	return (SUCCESS);
}