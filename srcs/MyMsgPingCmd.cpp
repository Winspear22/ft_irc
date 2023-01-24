# include "num_replies.hpp"

int			MyMsg::PingCmd( MyServer *IRC_Server )
{
	std::string msg_sent;
	if (this->Params.empty())
	{
		msg_sent = ERR_NOORIGIN(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		msg_sent = "PONG " + this->_SentFrom->_Nickname + " :" + this->Params.at(0);
		this->_SentFrom->SetClientsLastPing(std::time(0));
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	(void)IRC_Server;
	return (SUCCESS);
}