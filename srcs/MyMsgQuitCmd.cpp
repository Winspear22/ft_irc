# include "num_replies.hpp"

int			MyMsg::QuitCmd( MyServer *IRC_Server )
{
	unsigned int								i;
	std::string				 					msg_sent;
	std::map<Channels *, std::string>::iterator it;
	int											fd;

	fd = this->_SentFrom->GetClientsFd();
	msg_sent = "QUIT";
	i = -1;
	if (this->Params.empty())
		msg_sent = msg_sent +  " " + ":Client quit";
	if (this->Params.size() >= 1)
	{
		while (++i < this->Params.size())
			msg_sent = msg_sent + " " + this->Params[i];
	}
	it = IRC_Server->channels_list.begin();
	while (it != IRC_Server->channels_list.end())
	{
		if (it->first->GetClientsInChannelMemberList(this->_SentFrom->GetClientsNickname()) != NULL)
		{
			it->first->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
			it->first->DeleteClientsToChannelMemberList(this->_SentFrom);
		}
		it++;
	}
	SendMsgBackWithPrefix(*this, msg_sent);
	if (IRC_Server->GetClientsThroughSocketFd(this->_SentFrom->GetClientsFd()) != NULL && this->_SentFrom != NULL)
	{
		IRC_Server->clients_list.erase(IRC_Server->GetClientsThroughSocketFd(this->_SentFrom->GetClientsFd()));
		delete this->_SentFrom;
	}
	close(fd);
	FD_CLR(fd, &IRC_Server->ready_fds);
	IRC_Server->SetCurrentClientsNb(IRC_Server->GetCurrentClientsNb() - 1);
	return (SUCCESS);
}