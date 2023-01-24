# include "num_replies.hpp"

int			MyMsg::QuitCmd( MyServer *IRC_Server )
{
	unsigned int								i;
	std::string				 					msg_sent;
	std::map<Channels *, std::string>::iterator it;
	int											fd;

	fd = this->_SentFrom->GetClientsFd();
	msg_sent = "QUIT"; //message renvoyé selon le RFC modern.ircdocs.horse
	i = -1;
	if (this->Params.empty())
		msg_sent = msg_sent +  " " + ":Client quit";  //message renvoyé selon si le Client n'a pas spécifié de raison
	if (this->Params.size() >= 1)
	{
		while (++i < this->Params.size())
			msg_sent = msg_sent + " " + this->Params[i]; // message renvoyé si le client a spécifié une raison
	}
	it = IRC_Server->channels_list.begin();
	while (it != IRC_Server->channels_list.end())
	{
		if (it->first->GetClientsInChannelMemberList(this->_SentFrom->GetClientsNickname()) != NULL)
		{
			std::cout << "User supprimé du channel" << std::endl;
			it->first->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
			it->first->DeleteClientsToChannelMemberList(this->_SentFrom);
		}
		it++;
	}
	SendMsgBackWithPrefix(*this, msg_sent);
	if (IRC_Server->GetClientsThroughSocketFd(this->_SentFrom->GetClientsFd()) != NULL && this->_SentFrom != NULL)
	{
		IRC_Server->_clients_list.erase(IRC_Server->GetClientsThroughSocketFd(this->_SentFrom->GetClientsFd()));
		delete this->_SentFrom;
	}
	close(fd);
	FD_CLR(fd, &IRC_Server->ready_fds);
	IRC_Server->SetCurrentClientsNb(IRC_Server->GetCurrentClientsNb() - 1);
	std::cout << CYAN << "Current nb of Clients after QUIT : " << WHITE << IRC_Server->GetCurrentClientsNb() << NORMAL << std::endl;
	return (SUCCESS);
}