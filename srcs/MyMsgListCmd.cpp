#include <sstream>
# include "num_replies.cpp"

int		MyMsg::ListCmd( MyServer *IRC_Server )
{
	std::string 								msg_sent;
	char										*tmp;
	std::vector<std::string> 					channels;
	std::map<Channels*, std::string>::iterator	it1;
	std::vector<std::string>::iterator 			it2;
	Channels									*chan;
	std::string									name_chan;
	std::string									nb_members;
	size_t										nb_memberss;

	if (this->Params.empty())
	{
		it1 = IRC_Server->channels_list.begin();
		while (it1 != IRC_Server->channels_list.end())
		{
			chan = it1->first;
			name_chan = chan->GetChannelName();
			nb_memberss = chan->GetAllClientsInChannelMemberList().size();
			std::ostringstream convert;
			convert << nb_memberss;
			nb_members = convert.str();
			msg_sent = RPL_LIST(*this, name_chan, nb_members);
			SendMsgBackToClients(*this, msg_sent);
			it1++;
		}
	}
	else
	{
		tmp = strdup(this->Params.at(0).c_str());
		channels = IRC_Server->SplitByEndline(tmp, ",");
		it2 = channels.begin();
		free(tmp);
		while (it2 != channels.end())
		{
			if (IRC_Server->GetChannelsByName(*it2) != NULL)
			{
				name_chan = *it2;
				nb_memberss = IRC_Server->GetChannelsByName(*it2)->GetAllClientsInChannelMemberList().size();
				std::ostringstream convert;
				convert << nb_memberss;
				nb_members = convert.str();
				msg_sent = RPL_LIST(*this, name_chan, nb_members);
				SendMsgBackToClients(*this, msg_sent);
			}
			it2++;
		}
	}
	msg_sent = RPL_LISTEND(*this);
	SendMsgBackWithPrefix(*this, msg_sent);
	return (SUCCESS);
}