#ifndef MYMSG_HPP
# define MYMSG_HPP

# include "Clients.hpp"
# include "MyServer.hpp"
# include "Channels.hpp"

class Clients;
class MyServer;
class Channels;

class MyMsg
{
public:
    MyMsg( Clients  *popo, std::string lolo );
	MyMsg( const MyMsg & copy );
	~MyMsg( void );

	/*===========================================*/
	/*             GETTERS AND SETTERS           */
	/*--------------All the Setters--------------*/
	void 		SetPrefix( std::string );
	void 		SetCmd( std::string );
	void 		SetParams( std::string );
	void		SetCmdExistence( int CmdExistenceStatus );

	/*-------------------------------------------*/
	/*--------------All the Getters--------------*/
	std::string GetMsg( void );
	std::string GetPrefix( void );
	std::string GetCmd( void );
	Clients 	*GetClients( void );
	int			GetCmdExistence( void );
	/*-------------------------------------------*/
	/*===========================================*/

	int			CheckFormatCmd( std::vector<std::string>::iterator cmd, std::vector<std::string> cmd_list );

	/*===========================================*/
	/*                    COMMAND                */
	/*-------------------------------------------*/
	int			InfoCmd( void );
	int			MotdCmd( void );
	int			VersionCmd( void );
	int			PassCmd( MyServer *IRC_Server );
	int			NickCmd( MyServer *IRC_Server );
	int			UserCmd( MyServer *IRC_Server );
	int			ModeCmd( MyServer *IRC_Server );
	int			PingCmd( MyServer *IRC_Server );
	int			QuitCmd( MyServer *IRC_Server );
	int			JoinCmd( MyServer *IRC_Server );
	int			NamesCmd( MyServer *IRC_Server );
	int			ListCmd( MyServer *IRC_Server );
	int			KickCmd( MyServer *IRC_Server );
	int			PartCmd( MyServer *IRC_Server );
	int			OperCmd( MyServer *IRC_Server );
	int			TimeCmd( MyServer *IRC_Server );
	int			TopicCmd( MyServer *IRC_Server );
	int			NoticeCmd( MyServer *Irc_Server );
	int			PrivMsgCmd( MyServer *Irc_Server );
	int			WallopsCmd( MyServer *IRC_Server );
	int			ChanModeCmd( MyServer *IRC_Server );
	int			NamesCmd( MyServer *IRC_Server, MyMsg &msg );
	int			IsTheUserVisible( Channels *channel, Clients *sender );
	int			NickFormatCheck( std::vector<std::string>::iterator nickcheck );

	void		KillCmd( MyServer *IRC_Server );
	void		InviteCmd( MyServer *IRC_Server );
	/*-------------------------------------------*/
	/*===========================================*/

	int			ValidateClientsConnections( MyServer *IRC_Server );

	bool		ParseCmdInMyMsg( MyServer *IRC_Server );
	void 		SetParams2(std::vector<std::string> params);

	std::string					Prefix;
	std::string					Command;
	std::vector<std::string>	Params;
private:
    MyMsg();

	MyMsg & operator=( const MyMsg & popo );

	std::string 				_Message;
	Clients						*_SentFrom;
	int							_DoesCmdExist;
};
std::string toupper_striing( std::string cmd );

#endif