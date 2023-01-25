# include "num_replies.hpp"

int			MyMsg::IsTheUserVisible( Channels *channel, Clients *sender )
{
	std::map<Clients *, int>::iterator it;

	if (channel == NULL || sender == NULL)
		return false;
	it = channel->_MemberOfTheChannelList.begin();
	while (it != channel->_MemberOfTheChannelList.end())
	{
		if (it->first->GetClientsMode().find('i') == std::string::npos || it->first == sender)
			return (SUCCESS);
		it++;
	}
	return (FAILURE);
}

int			MyMsg::NamesCmd( MyServer *IRC_Server, MyMsg &msg )
{
	std::string msg_sent;

	if (msg.Params.size() == 0)
	{
		std::map<Channels *, std::string> chans = IRC_Server->channels_list;
		std::map<Channels *, std::string>::iterator it = chans.begin();
		while (it != chans.end())
		{
			std::map<Clients *, int> members = IRC_Server->GetChannelsByName(it->second)->GetAllClientsInChannelMemberList();
			std::map<Clients *, int>::iterator it2 = members.begin();
			msg_sent = RPL_NAMREPLY(*this, it->second);
			while (it2 != members.end())
			{
				if (it2->first->GetClientsMode().find('i') == std::string::npos)
					msg_sent += it2->first->GetClientsNickname() + " ";
				it2++;
			}
			SendMsgBackWithPrefix(msg, msg_sent);
			msg_sent = RPL_ENDOFNAMES(*this, it->second);
			SendMsgBackWithPrefix(msg, msg_sent);
			it++;
		}
		std::map<Clients *, int>::iterator EntireClientsList;
		std::map<Clients*, int> ClientsNotInAnyChannel;
		std::map<Clients*, int>::iterator it_ClientsNoyInAnyChannel;
		EntireClientsList = IRC_Server->clients_list.begin();
		it = chans.begin();
		if (it == chans.end())
		{
			while (EntireClientsList != IRC_Server->clients_list.end())
			{
				if (EntireClientsList->first->GetClientsMode().find('i') == std::string::npos)
					ClientsNotInAnyChannel.insert(std::make_pair(EntireClientsList->first, EntireClientsList->second));
			EntireClientsList++;
			}
		}
		else
		{
			while (EntireClientsList != IRC_Server->clients_list.end())
			{
				it = chans.begin();
				while (it != chans.end())
				{
					if (it->first->GetClientsInChannelMemberList(EntireClientsList->first->GetClientsNickname()) == NULL)
					{
						if (EntireClientsList->first->GetClientsMode().find('i') == std::string::npos)
							ClientsNotInAnyChannel.insert(std::make_pair(EntireClientsList->first, EntireClientsList->second));
					}
					it++;
				}
				EntireClientsList++;
			}
		}
		it_ClientsNoyInAnyChannel = ClientsNotInAnyChannel.begin();
		msg_sent = RPL_NAMREPLY(*this, "*");
		while (it_ClientsNoyInAnyChannel != ClientsNotInAnyChannel.end())
		{
			msg_sent += it_ClientsNoyInAnyChannel->first->GetClientsNickname() + " ";
			it_ClientsNoyInAnyChannel++;
		}
		SendMsgBackWithPrefix(msg, msg_sent);
		msg_sent = RPL_ENDOFNAMES(*this, "*");
		SendMsgBackWithPrefix(msg, msg_sent);
	}
	else
	{
		char *tmp = strdup(msg.Params[0].c_str());
		std::vector<std::string> channels = IRC_Server->SplitByEndline(tmp, ",");
		std::vector<std::string>::iterator it = channels.begin();
		free(tmp);
		while (it != channels.end())
		{
			if (IRC_Server->GetChannelsByName(*it) == NULL)
				it++;
			else
			{
				if (msg.IsTheUserVisible(IRC_Server->GetChannelsByName(*it), msg._SentFrom) == SUCCESS)
				{
					std::map<Clients *, int> members = IRC_Server->GetChannelsByName(*it)->_MemberOfTheChannelList;
					std::map<Clients *, int>::iterator it2 = members.begin();
					msg_sent = RPL_NAMREPLY(*this, *it);
					while (it2 != members.end())
					{
						if (it2->first->GetClientsMode().find('i') == std::string::npos || it2->first == msg._SentFrom || IRC_Server->GetChannelsByName(*it)->GetClientsInChannelMemberList(msg._SentFrom->GetClientsNickname()))
						{
							if (IRC_Server->GetChannelsByName(*it)->GetChannelCreator() != NULL)
							{
								if (it2->first == IRC_Server->GetChannelsByName(*it)->GetChannelCreator())
									msg_sent += "@";
								msg_sent += it2->first->GetClientsNickname() + " ";
							}
						}
						it2++;
					}			
				}
				if (IRC_Server->GetChannelsByName(*it) != NULL)
				{
					SendMsgBackWithPrefix(msg, msg_sent);
					msg_sent = RPL_ENDOFNAMES(*this, *it);
					SendMsgBackWithPrefix(msg, msg_sent);
				}
				it++;
			}
		}
	}
	return (SUCCESS);
}