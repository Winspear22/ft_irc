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

	/*===========================================*/
	/*             GETTERS AND SETTERS           */
	/*--------------All the Setters--------------*/
	void						SetChannelMode(std::string mode);
	void						SetChannelName(std::string name);
	void						SetHasAChannelCreator(int status);
	void						SetChannelstopic(std::string topic);
	/*-------------------------------------------*/
	/*--------------All the Getters--------------*/
	int							GetHasAChannelCreator( void );
	std::string					GetChannelName( void );
	std::string					GetChannelMode( void );
	std::string					GetChannelstopic( void );
	std::map<Clients *, int> 	GetAllowedToInviteList( void );
	std::map<Clients*, int> 	GetAllClientsInChannelMemberList( void );
	Clients						*GetChannelCreator( void );
	Clients						*GetClientsInChannelMemberList( std::string NickName );
	Clients						*GetClientsAllowedToInvite( std::string name );
	/*-------------------------------------------*/
	/*===========================================*/

	void						AddClientsToChannelMemberList( Clients *client );
	void						DeleteClientsToChannelMemberList( Clients *client );
	void						SendMsgToAllInChannels( MyMsg *msg, std::string msg_sent, Clients *SentFrom );
	void						SendMsgToAllInChannelsForTopic( MyMsg *msg, std::string msg_sent, Clients *SentFrom);

	int							AddChannelsMode( char AddMode );
	int							DeleteChannelsMode( char DeleteMode );
	void						AddClientsToAllowedToInviteList( Clients *client );
	void						DeleteClientsFromAllowedToInviteList( Clients *client );

	std::map<Clients*, int> 	_MemberOfTheChannelList;
	std::string					_ChannelName;
	Clients						*_CreatedBy;

private:
    Channels(/*ARG*/);
	Channels( const Channels & copy );
	Channels & operator=( const Channels & popo );

	std::map<Clients*, int> 	_IsAllowedToInvite;
	int							_HasAChannelCreator;
	std::string					_Topic;
	std::string					_ChanMode;

};

#endif