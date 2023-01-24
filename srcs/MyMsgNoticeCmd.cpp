# include "num_replies.hpp"
int	MyMsg::NoticeCmd( MyServer *Irc_Server )
{
	std::string msg_sent;
	int			ret_send;
	int			ret_find_first_of;
	std::string	tmp;
	size_t		ping_pos;

	tmp = "";
	ret_find_first_of = this->Params.at(0).find_first_of("!#+&");
	if (this->Params.size() < 2)
		return (FAILURE);
	else if (Irc_Server->GetClientsThroughName(this->Params.at(0)) == NULL && ret_find_first_of != 0)
		return (FAILURE);
	else if (Irc_Server->GetChannelsByName(this->Params.at(0)) == NULL && ret_find_first_of == 0)
		return (FAILURE);
	else
	{
		ping_pos = this->_SentFrom->GetClientsMessage().find(':', this->Prefix.size() + this->Command.size() + this->Params.at(0).size());
		if (ping_pos != std::string::npos)
			tmp = this->_Message.substr(ping_pos);
		if (ret_find_first_of == 0)
		{
			msg_sent = "NOTICE " + this->Params.at(0) + " " + tmp;
			Irc_Server->GetChannelsByName(this->Params.at(0))->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
		}
		else
		{
			msg_sent = this->GetPrefix() + " NOTICE " + this->Params[0] + " " + tmp + "\r\n";
			ret_send = send(Irc_Server->GetClientsThroughName(this->Params[0])->GetClientsFd(), msg_sent.c_str(), strlen(msg_sent.c_str()), MSG_DONTWAIT);
			if (ret_send == ERROR_SERVER)
				std::cerr << RED << "Error" << WHITE << " send(); " << RED << "in NOTICE did not send anything." << NORMAL << std::endl;
		}
	}
	return (SUCCESS);
}