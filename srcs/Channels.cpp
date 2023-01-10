# include "Channels.hpp"

Channels::Channels( void )
{
	std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}

Channels::Channels( Clients *CreatedBy, std::string ChannelName )
{
	std::cout << PURPLE << "Channels constructor used." << NORMAL << std::endl;
	this->_ChannelName = ChannelName;
	this->_CreatedBy = CreatedBy;
	return ;
}


Channels::Channels( const Channels & copy )
{
	std::cout << "\033[0;33mChannels Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

Channels & Channels::operator=( Channels const & rhs )
{
	std::cout << "\033[0;34mChannels Copy assignment operator called." << NORMAL << std::endl;
	this->_ChannelName = rhs._ChannelName;
	this->_CreatedBy = rhs._CreatedBy;
	return (*this);
}


Channels::~Channels( void )
{
	std::cout << RED << "Channels Destructor called." << NORMAL << std::endl;
	
	
	return ;
}

std::string			Channels::GetChannelName( void )
{
	return (this->_ChannelName);
}

void				Channels::SetChannelName( std::string name )
{
	this->_ChannelName = name;
}

Clients				*Channels::GetChannelCreator( void )
{
	return (this->_CreatedBy);
}

Clients		*Channels::GetClientsInChannelMemberList( std::string NickName )
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

std::map<Clients*, int> Channels::GetAllClientsInChannelMemberList( void )
{
	return (this->_MemberOfTheChannelList);
}

void		Channels::AddClientsToChannelMemberList( Clients *client )
{
	if (client == NULL)
		return ;
	else
		this->_MemberOfTheChannelList.insert(std::make_pair(client, client->GetClientsFd()));
}

void		Channels::DeleteClientsToChannelMemberList( Clients *client )
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
				this->_MemberOfTheChannelList.erase(client);
			it++;
		}	
	}
}

void		Channels::SendMsgToAllInChannels( MyMsg *msg, std::string msg_sent, Clients *SentFrom )
{
	std::map<Clients*, int>::iterator it;
	int									ret_send;

	it = this->_MemberOfTheChannelList.begin();
	msg_sent = msg->GetPrefix() + " " + msg_sent;
	while (it != this->_MemberOfTheChannelList.end())
	{
		if (it->first->GetClientsNickname() != SentFrom->GetClientsNickname())
		{	
			std::cout << "Send msgto all = " << it->first->GetClientsNickname() << std::endl;
			std::cout << "msg sent in broadcast == " << msg_sent << std::endl;
			std::cout << "msg == " << msg->GetClients()->GetClientsNickname() << std::endl;
			std::cout << "FD == " << msg->GetClients()->GetClientsFd() << std::endl;
			ret_send = send(it->first->GetClientsFd(), msg_sent.c_str(), strlen(msg_sent.c_str()), MSG_DONTWAIT);
			if (ret_send == ERROR_SERVER)
				return(loop_errors_handlers_msg(ERROR_SEND));
		//	SendMsgBackWithPrefix(*msg, msg_sent);
		}
		it++;
	}
}

