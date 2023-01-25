# include "num_replies.hpp"

int		MyMsg::VersionCmd( void )
{
	std::vector<std::string>::iterator it;
	
	it = this->Params.begin();
	if (this->Params.empty())
		SendMsgBackToClients(*this, RPL_VERSION(*this));
	else if (this->Params.size() == 1)
		SendMsgBackToClients(*this, ERR_NOSUCHSERVER(*this, it));
	return (SUCCESS);
}