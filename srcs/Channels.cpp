# include "Channels.hpp"
# include "MyMsg.hpp"
# include "MyServer.hpp"
# include "Clients.hpp"

Channels::Channels(void)
{
	return ;
}

Channels::Channels(Clients *CreatedBy, std::string ChannelName)
{
	this->_ChannelName = ChannelName;
	this->_CreatedBy = CreatedBy;
	this->_HasAChannelCreator = YES;
	return ;
}

Channels::Channels(const Channels & copy)
{
	*this = copy;
    return ;
}

Channels & Channels::operator=(Channels const & rhs)
{
	this->_ChannelName = rhs._ChannelName;
	this->_CreatedBy = rhs._CreatedBy;
	return (*this);
}

Channels::~Channels( void )
{
	return ;
}

	/*===========================================*/
	/*===========================================*/
	/*             GETTERS AND SETTERS           */
	/*--------------All the Setters--------------*/
	/*             GETTERS AND SETTERS           */
	/*--------------All the Setters--------------*/
void						Channels::SetChannelMode( std::string mode )
{
	this->_ChanMode = mode;
}

void						Channels::SetChannelName( std::string name )
{
	this->_ChannelName = name;
}

void						Channels::SetHasAChannelCreator( int status )
{
	this->_HasAChannelCreator = status;
}

void						Channels::SetChannelstopic( std::string topic )
{
	this->_Topic = topic;
}

	/*-------------------------------------------*/
	/*--------------All the Getters--------------*/

int							Channels::GetHasAChannelCreator( void )
{
	return (this->_HasAChannelCreator);
}

std::string					Channels::GetChannelName( void )
{
	return (this->_ChannelName);
}

std::string					Channels::GetChannelMode( void )
{
	return (this->_ChanMode);
}

std::string					Channels::GetChannelstopic( void )
{
	return (this->_Topic);
}

std::map<Clients *, int> 	Channels::GetAllowedToInviteList( void )
{
	return (this->_IsAllowedToInvite);
}

std::map<Clients*, int> 	Channels::GetAllClientsInChannelMemberList( void )
{
	return (this->_MemberOfTheChannelList);
}

Clients						*Channels::GetChannelCreator( void )
{
	return (this->_CreatedBy);
}

	/*-------------------------------------------*/
	/*===========================================*/

int							Channels::DeleteChannelsMode( char DeleteMode )
{
	if (this->_ChanMode.find(DeleteMode) == std::string::npos)
		return (FAILURE);
	else
		this->_ChanMode.erase(this->_ChanMode.find(DeleteMode), 1);
	return (SUCCESS);

}

int							Channels::AddChannelsMode( char AddMode )
{
	if (this->_ChanMode.find(AddMode) != std::string::npos)
		return (FAILURE);
	else
		this->_ChanMode.push_back(AddMode);

	return (SUCCESS);
}

Clients						*Channels::GetClientsAllowedToInvite( std::string NickName )
{
	std::map<Clients*, int>::iterator it;

	it = this->_IsAllowedToInvite.begin();
	while (it != this->_IsAllowedToInvite.end())
	{
		if (it->first->GetClientsNickname() == NickName)
			return (it->first);
		it++;
	}
	return (NULL);
}

void						Channels::AddClientsToAllowedToInviteList( Clients *client )
{
	if (client == NULL)
		return ;
	else
		this->_IsAllowedToInvite.insert(std::make_pair(client, client->GetClientsFd()));

}

void						Channels::DeleteClientsFromAllowedToInviteList( Clients *client )
{
	std::map<Clients*, int>::iterator it;

	it = this->_IsAllowedToInvite.begin();
	if (client == NULL)
		return ;
	else
	{
		while (it != this->_IsAllowedToInvite.end())
		{
			if (client == it->first)
			{
				this->_IsAllowedToInvite.erase(client);
				break ;
			}
			it++;
		}	
	}
}

Clients						*Channels::GetClientsInChannelMemberList( std::string NickName )
{
	std::map<Clients*, int>::iterator it;

	it = this->_MemberOfTheChannelList.begin();
	while (it != this->_MemberOfTheChannelList.end())
	{
		if (it->first->GetClientsNickname() == NickName)
			return (it->first);
		it++;
	}
	return (NULL);
}


void						Channels::AddClientsToChannelMemberList( Clients *client )
{
	if (client == NULL)
		return ;
	else
		this->_MemberOfTheChannelList.insert(std::make_pair(client, client->GetClientsFd()));
}

void						Channels::DeleteClientsToChannelMemberList( Clients *client )
{
	std::map<Clients*, int>::iterator it;

	it = this->_MemberOfTheChannelList.begin();
	if (client == NULL)
		return ;
	else
	{
		while (it != this->_MemberOfTheChannelList.end())
		{
			if (client == it->first)
			{
				this->_MemberOfTheChannelList.erase(client);
				break ;
			}
			it++;
		}	
	}
}

void						Channels::SendMsgToAllInChannels( MyMsg *msg, std::string msg_sent, Clients *SentFrom )
{
	std::map<Clients*, int>::iterator	it;
	int									ret_send;

	it = this->_MemberOfTheChannelList.begin();
	msg_sent = msg->GetPrefix() + " " + msg_sent + "\r\n";
	while (it != this->_MemberOfTheChannelList.end())
	{
		if (it->first->GetClientsNickname() != SentFrom->GetClientsNickname())
		{	
			ret_send = send(it->first->GetClientsFd(), msg_sent.c_str(), strlen(msg_sent.c_str()), MSG_DONTWAIT);
			if (ret_send == ERROR_SERVER)
				return(loop_errors_handlers_msg(ERROR_SEND));
		}
		it++;
	}
}

void						Channels::SendMsgToAllInChannelsForTopic( MyMsg *msg, std::string msg_sent, Clients *SentFrom )
{
	std::map<Clients*, int>::iterator	it;
	int									ret_send;
(void)SentFrom;
	it = this->_MemberOfTheChannelList.begin();
	msg_sent = msg->GetPrefix() + " " + msg_sent + "\r\n";
	while (it != this->_MemberOfTheChannelList.end())
	{
			ret_send = send(it->first->GetClientsFd(), msg_sent.c_str(), strlen(msg_sent.c_str()), MSG_DONTWAIT);
			if (ret_send == ERROR_SERVER)
				return(loop_errors_handlers_msg(ERROR_SEND));
		it++;
	}
}