# include "num_replies.cpp"

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
	std::string str;

	if (msg.Params.size() == 0)
	{
		std::map<Channels *, std::string> chans = IRC_Server->channels_list;
		std::map<Channels *, std::string>::iterator it = chans.begin();
		while (it != chans.end())
		{
			std::map<Clients *, int> members = IRC_Server->GetChannelsByName(it->second)->_MemberOfTheChannelList;
			std::map<Clients *, int>::iterator it2 = members.begin();
			str = "353";
			str += " " + msg._SentFrom->_Nickname + " =" + it->second + " :";
			while (it2 != members.end())
			{
				str += it2->first->_Nickname + " ";
				it2++;
			}
			SendMsgBackWithPrefix(msg, str);
			str = "366";
			str += " " + msg._SentFrom->_Nickname + " ";
			str += it->second + ":End of /NAMES list";
			SendMsgBackWithPrefix(msg, str);
			it++;
		}
	}
	else
	{
		char *tmp = strdup(msg.Params[0].c_str());
		std::vector<std::string> channels = IRC_Server->SplitByEndline(tmp, ","); //changer ici vector->list
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
					str = "353";
					str += " " + msg._SentFrom->_Nickname + " = " + *it + " :";
					while (it2 != members.end())
					{
						if (it2->first->GetClientsMode().find('i') == std::string::npos || it2->first == msg._SentFrom || IRC_Server->GetChannelsByName(*it)->GetClientsInChannelMemberList(msg._SentFrom->_Nickname))
						{
							if (IRC_Server->GetChannelsByName(*it)->GetChannelCreator() != NULL)
							{
								if (it2->first == IRC_Server->GetChannelsByName(*it)->GetChannelCreator()) //changer ici + fd
									str += "@";
								str += it2->first->_Nickname + " ";
							}
						}
						it2++;
					}			
				}
				if (IRC_Server->GetChannelsByName(*it) != NULL)
				{
					SendMsgBackWithPrefix(msg, str);
					str = "366";
					str += " " + msg._SentFrom->_Nickname + " ";
					str += *it + " :End of /NAMES list";
					SendMsgBackWithPrefix(msg, str);
				}
				it++;
			}
		}
	}
	return (SUCCESS);
}