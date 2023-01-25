# include "num_replies.hpp"

int MyMsg::TimeCmd( MyServer *IRC_Server )
{
	(void)IRC_Server;
	std::string msg_sent;

	if (this->Params.size() == 1 && this->_SentFrom->GetServerName() != this->Params.at(0))
	{
		msg_sent = "402 :" + this->_SentFrom->GetServerName() + ": No such server";
		SendMsgBackWithPrefix(*this, msg_sent);

	}
	else if (this->Params.size() == 1 && this->_SentFrom->GetServerName() == this->Params.at(0))
	{
		msg_sent = RPL_TIME(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.empty())
	{
		msg_sent = RPL_TIME(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	return (SUCCESS);
}