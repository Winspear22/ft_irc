# include "num_replies.hpp"

void		MyMsg::KillCmd( MyServer *IRC_Server )
{
	std::string		msg_sent;
	std::string		msg_sent2;
	int				ret_send;
	size_t			i;

	if (this->Params.empty())
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->GetClients()->GetClientsMode().find('o') == std::string::npos)
	{
		msg_sent = ERR_NOPRIVILEGES(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (IRC_Server->GetClientsThroughName(this->Params.at(0)) == NULL)
	{
		msg_sent = ERR_NOSUCHNICK(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		msg_sent = ":" + this->Params.at(0) + "!" + IRC_Server->GetClientsThroughName(this->Params.at(0))->GetClientsUsername() + "@" + IRC_Server->GetClientsThroughName(this->Params.at(0))->GetClientsHostname() + " ";
		msg_sent += "KILL " + this->Params.at(0);
		if (this->Params.size() > 1)
		{
			i = 0;
			while (++i < this->Params.size())
				msg_sent = msg_sent + " " + this->Params[i];
		}
		msg_sent += "\r\n";
		ret_send = send(IRC_Server->GetClientsThroughName(this->Params.at(0))->GetClientsFd(), msg_sent.c_str(), strlen(msg_sent.c_str()), MSG_DONTWAIT);
		if (ret_send == ERROR_SERVER)
			return (loop_errors_handlers_msg(ERROR_SEND));
		IRC_Server->SetUnavailableNickname(this->Params.at(0));
		MyMsg msg(IRC_Server->GetClientsThroughName(this->Params.at(0)), "QUIT killed by " + this->_SentFrom->GetClientsNickname());
		msg.parse_msg();
		msg.QuitCmd(IRC_Server);
		std::map<Channels*, std::string>::iterator it_chan;
		std::map<Clients*, int>::iterator it_client;
		it_chan = IRC_Server->channels_list.begin();
		it_client = IRC_Server->clients_list.begin();
		while (it_chan != IRC_Server->channels_list.end())
		{
			it_chan->first->DeleteClientsToChannelMemberList(IRC_Server->GetClientsThroughName(this->Params.at(0)));
			it_chan++;
		}
		while (it_client != IRC_Server->clients_list.end())
		{
			if (it_client->first->GetClientsNickname() != this->Params.at(0))
			{
				ret_send = send(it_client->first->GetClientsFd(), msg_sent2.c_str(), strlen(msg_sent2.c_str()), MSG_DONTWAIT);
				if (ret_send == ERROR_SERVER)
					return (loop_errors_handlers_msg(ERROR_SEND));
			}
			it_client++;
		}
	}
}