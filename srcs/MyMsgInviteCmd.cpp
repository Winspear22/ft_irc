# include "num_replies.hpp"

void		MyMsg::InviteCmd( MyServer *IRC_Server )
{
	std::string		nick;
	std::string		chan;
	std::string		msg_sent;
	int				ret_send;
	std::string		prefix;

	if (this->Params.size() < 2)
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (IRC_Server->GetClientsThroughName(this->Params.at(0)) == NULL)
	{
		msg_sent = ERR_NOSUCHNICK(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (IRC_Server->GetChannelsByName(this->Params.at(1)) == NULL)
	{
		//ne rien faire
	}
	else if (IRC_Server->GetChannelsByName(this->Params.at(1))->GetClientsInChannelMemberList(this->GetClients()->GetClientsNickname()) == NULL)
	{
		msg_sent = ERR_NOTONCHANNEL(*this, this->Params.at(1));
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (IRC_Server->GetChannelsByName(this->Params.at(1))->GetClientsInChannelMemberList(this->Params.at(0)) != NULL)
	{
		msg_sent = ERR_USERONCHANNEL(*this, this->Params.at(0), this->Params.at(1));
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		prefix = ":" + IRC_Server->GetClientsThroughName(this->Params.at(0))->GetClientsNickname() + "!" + IRC_Server->GetClientsThroughName(this->Params.at(0))->GetClientsUsername() + "@" + IRC_Server->GetClientsThroughName(this->Params.at(0))->_Hostname;
		msg_sent = RPL_INVITING(*this, this->Params.at(1), this->Params.at(0));
		SendMsgBackWithPrefix(*this, msg_sent);
		prefix += " " + msg_sent;
		prefix += "\r\n";
		ret_send = send(IRC_Server->GetClientsThroughName(this->Params.at(0))->GetClientsFd(), prefix.c_str(), strlen(prefix.c_str()), MSG_DONTWAIT);
		if (ret_send == ERROR_SERVER)
			return (loop_errors_handlers_msg(ERROR_SEND));
	}
}