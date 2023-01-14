#ifndef CHANNELS_HPP
# define CHANNELS_HPP

//# include "irc.hpp"
# include "Clients.hpp"
# include "MyMsg.hpp"

class MyMsg;
class MyServer;
class Clients;

class Channels
{
public:
    Channels( Clients *CreatedBy, std::string ChannelName );
	Channels( const Channels & copy );
	Channels & operator=( const Channels & popo );
	~Channels( void );

	std::string				GetChannelName( void );
	void					SetChannelName( std::string name );
	Clients					*GetChannelCreator( void );
	Clients					*GetClientsInChannelMemberList( std::string NickName );
	std::map<Clients*, int> GetAllClientsInChannelMemberList( void );


	void					AddClientsToChannelMemberList( Clients *client );
	void					DeleteClientsToChannelMemberList( Clients *client );
	void					SendMsgToAllInChannels( MyMsg *msg, std::string msg_sent, Clients *SentFrom );
	std::map<Clients*, int> _MemberOfTheChannelList;
	std::string				_ChannelName;
	Clients					*_CreatedBy;

private:
    Channels(/*ARG*/);

	//std::string				_ChannelName;
	//std::map<Clients*, int> _MemberOfTheChannelList;
	//Clients					*_CreatedBy;
};

#endif