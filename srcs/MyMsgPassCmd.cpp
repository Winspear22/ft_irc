# include "num_replies.hpp"

int		MyMsg::PassCmd( MyServer *IRC_Server )
{
	std::string msg_sent;
	std::vector<std::string>::iterator it;
	it = this->Params.begin();
	if (this->Params.empty())
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		this->_SentFrom->SetClientsConnectionAuthorisation(YES);
		SendMsgBackToClients(*this, msg_sent);
		this->_SentFrom->SetClientsConnectionAuthorisation(NO);
		this->_SentFrom->SetClientsConnectionStatus(NO);
	}
	else if (*it != IRC_Server->GetPassword())
	{
		msg_sent = ERR_PASSWDMISMATCH();
		this->_SentFrom->SetClientsConnectionAuthorisation(YES);
		SendMsgBackToClients(*this, msg_sent);
		this->_SentFrom->SetClientsConnectionAuthorisation(NO);
		this->_SentFrom->SetClientsConnectionStatus(NO);

	}	
	else if (!this->Params.empty() && this->_SentFrom->GetClientsConnectionAuthorisation() == YES)
	{
		msg_sent = ERR_ALREADYREGISTRED(*this);
		SendMsgBackToClients(*this, msg_sent);
	}
	else if (*it == IRC_Server->GetPassword())
	{
		std::cout << "Pass validé" << std::endl;
		this->_SentFrom->SetClientsConnectionAuthorisation(YES);
	}
	return (SUCCESS);
}