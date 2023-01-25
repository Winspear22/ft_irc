#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include "irc.hpp"

class MyMsg;
class MyServer;
class Clients;

class Channels
{
public:
    Channels( Clients *CreatedBy, std::string ChannelName );
	~Channels( void );

	std::string				GetChannelName( void );
	void					SetChannelName( std::string name );
	Clients					*GetChannelCreator( void );
	Clients					*GetClientsInChannelMemberList( std::string NickName );
	std::map<Clients*, int> GetAllClientsInChannelMemberList( void );
	std::string				GetChannelstopic( void );
	void					SetChannelstopic( std::string topic );

	void					AddClientsToChannelMemberList( Clients *client );
	void					DeleteClientsToChannelMemberList( Clients *client );
	void					SendMsgToAllInChannels( MyMsg *msg, std::string msg_sent, Clients *SentFrom );
	void					SendMsgToAllInChannelsForTopic( MyMsg *msg, std::string msg_sent, Clients *SentFrom);

	int						GetHasAChannelCreator( void );
	void					SetHasAChannelCreator( int status );
	std::string				GetChannelMode( void );
	void					SetChannelMode( std::string mode );
	int						DeleteChannelsMode( char DeleteMode );
	int						AddChannelsMode( char AddMode );
	std::map<Clients *, int> GetAllowedToInviteList( void );
	Clients					*GetClientsAllowedToInvite( std::string name );
	void					AddClientsToAllowedToInviteList( Clients *client );
	void					DeleteClientsFromAllowedToInviteList( Clients *client );



	std::map<Clients*, int> _MemberOfTheChannelList;
	std::string				_ChannelName;
	Clients					*_CreatedBy;

private:
    Channels(/*ARG*/);
	Channels( const Channels & copy );
	Channels & operator=( const Channels & popo );

	std::map<Clients*, int> _IsAllowedToInvite;
	int						_HasAChannelCreator;
	std::string				_Topic;
	std::string				_ChanMode;

};

#endif