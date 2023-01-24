# include "num_replies.hpp"

int		MyMsg::OperCmd( MyServer *IRC_Server )
{
	std::string msg_sent;
	(void)IRC_Server;
	

	if (this->Params.size() < 2)
	{
		msg_sent = ERR_NEEDMOREPARAMS2(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.at(0) != ID_OF_OPERATORS || this->Params.at(1) != PASSWORD_OF_OPERATORS)
	{
		msg_sent = ERR_PASSWDMISMATCH();
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		msg_sent = RPL_YOUREOPER(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
		this->_SentFrom->AddClientsMode('o');
	}
	return (SUCCESS);
}