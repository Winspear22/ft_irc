# include "num_replies.hpp"

int		MyMsg::JoinCmd( MyServer *IRC_Server )
{
	std::string 						msg_sent;
	char								*tmp;
	std::vector<std::string> 			channels;
	std::vector<std::string>::iterator	it;
	Channels							*chan;

	if (this->Params.empty())
	{
		msg_sent = ERR_NEEDMOREPARAMS(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		tmp = strdup(this->Params.at(0).c_str());
		channels = IRC_Server->SplitByEndline(tmp, ",");
		it = channels.begin();
		free(tmp);
		while (it != channels.end())
		{
			if ((*it)[0] != '#' && (*it)[0] != '&' && (*it)[0] != '+' && (*it)[0] != '!')
				it->insert(it->begin(), '#');
			if (it->size() > 50) //CHANLIMIT
				it->resize(50);
			if (IRC_Server->GetChannelsByName(*it) == NULL)
			{
				chan = IRC_Server->CreateChannels(*it, this->_SentFrom);//new Channels(this->_SentFrom, *it);
				chan->AddClientsToChannelMemberList(this->_SentFrom);
				msg_sent = "JOIN " + *it;
				SendMsgBackWithPrefix(*this, msg_sent);
				chan->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
				MyMsg UseOfNamesCmd(this->_SentFrom, "NAMES " + *it);
				UseOfNamesCmd.parse_msg();
				NamesCmd(IRC_Server, UseOfNamesCmd);
			}
			else
			{
				IRC_Server->GetChannelsByName(*it)->AddClientsToChannelMemberList(this->_SentFrom);
				msg_sent = "JOIN " + *it;
				SendMsgBackWithPrefix(*this, msg_sent);
				IRC_Server->GetChannelsByName(*it)->SendMsgToAllInChannels(this, msg_sent, this->_SentFrom);
				MyMsg UseOfNamesCmd(this->_SentFrom, "NAMES " + *it);
				UseOfNamesCmd.parse_msg();
				NamesCmd(IRC_Server, UseOfNamesCmd);
			}
			it++;
		}
	}
	
	return (SUCCESS);
}