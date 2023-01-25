# include "num_replies.hpp"
# include "num_replies.cpp"

int		MyMsg::WallopsCmd( MyServer *IRC_Server )
{
	std::string msg_sent;

	if (this->Params.empty())
	{
		msg_sent = ERR_NEEDMOREPARAMS2(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->_SentFrom->GetClientsMode().find('o') == std::string::npos)
	{
		msg_sent = ERR_NOPRIVILEGES(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->_SentFrom->GetClientsMode().find('o') != std::string::npos)
	{
		size_t ping_pos = this->_Message.find(':', this->Prefix.size() + this->Command.size());
		std::string tmp = "";
		if (ping_pos != std::string::npos)
			tmp = this->_Message.substr(ping_pos);
		msg_sent = this->Prefix + " WALLOPS " + tmp;
		std::map<Clients*, int>::iterator it;
		it = IRC_Server->clients_list.begin();
		int ret_send;
		msg_sent += "\r\n";
		while (it != IRC_Server->clients_list.end())
		{
			if (it->first->GetClientsMode().find('w') != std::string::npos)
			{
				ret_send = send(it->second, msg_sent.c_str(), strlen(msg_sent.c_str()), MSG_DONTWAIT);
				if (ret_send == ERROR_SERVER)
					std::cerr << RED << "Error, send() in WALLOPS" << NORMAL << std::endl;
			}
			it++;
		}
	}
	return (SUCCESS);
}