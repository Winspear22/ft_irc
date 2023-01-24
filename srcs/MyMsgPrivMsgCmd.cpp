# include "num_replies.hpp"

int	MyMsg::PrivMsgCmd( MyServer *Irc_Server )
{
	std::string	msg_sent;
	size_t		ping_pos;
	int 		ret_send;
	std::string	tmp;
	
	tmp = "";
	if (this->Params.size() == 0)
		msg_sent = ERR_NORECIPENT(*this);
	else if (this->Params.size() == 1)
		msg_sent = ERR_NOTEXTTOSEND(*this);
	else if (Irc_Server->GetClientsThroughName(this->Params[0]) == NULL && this->Params[0].find_first_of("!#+&") != 0)
		msg_sent = ERR_NOSUCHNICK(*this);
	else if (Irc_Server->GetChannelsByName(this->Params[0]) == NULL && this->Params[0].find_first_of("!#+&") == 0)
		msg_sent = ERR_NOSUCHNICK(*this);
	else
	{
		ping_pos = this->_SentFrom->GetClientsMessage().find(':', this->Prefix.size() + this->Command.size() + this->Params.at(0).size());
		if (ping_pos != std::string::npos)
			tmp = this->_SentFrom->GetClientsMessage().substr(ping_pos);
		if (this->Params[0].find_first_of("!#+&") == 0)
		{
			msg_sent = "PRIVMSG " + this->Params.at(0) + " " + tmp;
			Irc_Server->GetChannelsByName(this->Params.at(0))->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
			return (SUCCESS);
		}
		else
		{
			msg_sent = this->GetPrefix() + " PRIVMSG " + this->Params[0] + " " + tmp + "\r\n";
			ret_send = send(Irc_Server->GetClientsThroughName(this->Params[0])->GetClientsFd(), msg_sent.c_str(), strlen(msg_sent.c_str()), MSG_DONTWAIT);
			if (ret_send == ERROR_SERVER)
				std::cerr << RED << "Error" << WHITE << " send(); " << RED << "in PRIVMSG did not send anything." << NORMAL << std::endl;
			return (SUCCESS);
		}
	}
	SendMsgBackWithPrefix(*this, msg_sent);
	return (SUCCESS);
}

