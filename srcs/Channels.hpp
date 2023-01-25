#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include "irc.hpp"

class MyMsg;
class MyServer;
class Clients;

class Channels
{
public:
    Channels(Clients *CreatedBy, std::string ChannelName );
	~Channels( void );

	std::map<Clients*, int> _MemberOfTheChannelList;
	std::string				_ChannelName;
	Clients					*_CreatedBy;

	/*===========================================*/
	/*             GETTERS AND SETTERS           */
	/*--------------All the Setters--------------*/
	void					SetChannelName( std::string name );
	void					SetChannelMode( std::string mode );
	void					SetHasAChannelCreator( int status );
	void					SetChannelstopic( std::string topic );
	/*-------------------------------------------*/
	/*--------------All the Getters--------------*/
	int						GetHasAChannelCreator( void );

	Clients					*GetChannelCreator( void );
	Clients					*GetClientsInChannelMemberList( std::string NickName );

	std::string				GetChannelName( void );
	std::string				GetChannelstopic( void );

	std::map<Clients *, int> GetAllowedToInviteList( void );
	std::map<Clients*, int> GetAllClientsInChannelMemberList( void );
	/*-------------------------------------------*/
	/*===========================================*/
	void					AddClientsToChannelMemberList( Clients *client );
	void					DeleteClientsToChannelMemberList( Clients *client );
	void					SendMsgToAllInChannels( MyMsg *msg, std::string msg_sent, Clients *SentFrom );
	void					SendMsgToAllInChannelsForTopic( MyMsg *msg, std::string msg_sent, Clients *SentFrom);

	std::string				GetChannelMode( void );
	int						DeleteChannelsMode( char DeleteMode );
	int						AddChannelsMode( char AddMode );
	Clients					*GetClientsAllowedToInvite( std::string name );
	void					AddClientsToAllowedToInviteList( Clients *client );
	void					DeleteClientsFromAllowedToInviteList( Clients *client );

private:

    Channels(void);
	Channels(const Channels & copy);
	Channels & operator=( const Channels & popo );

	int						_HasAChannelCreator;
	std::string				_Topic;
	std::string				_ChanMode;
	std::map<Clients*, int> _IsAllowedToInvite;

};

#endif