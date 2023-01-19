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
	MyMsg & operator=( const MyMsg & popo );
	~MyMsg( void );

	std::string GetMsg( void );
	std::string GetPrefix( void );
	std::string GetCmd( void );
	Clients 	*GetClients( void );
	int			GetCmdExistence( void );
	//std::string GetParams( void );
	void 		SetPrefix( std::string );
	void 		SetCmd( std::string );
	void 		SetParams( std::string );
	void		SetCmdExistence( int CmdExistenceStatus );
	int			CheckFormatCmd( std::vector<std::string>::iterator cmd, std::vector<std::string> cmd_list );

	int			PassCmd( MyServer *IRC_Server );
	int			NickCmd( MyServer *IRC_Server );
	int			NickFormatCheck( std::vector<std::string>::iterator nickcheck );
	//int			NickFormatCheck( std::string nickcheck );
	int			UserCmd( MyServer *IRC_Server );
	int			MotdCmd( void );
	int			ModeCmd( MyServer *IRC_Server );
	int			PingCmd( MyServer *IRC_Server );
	int			VersionCmd( void );
	int			QuitCmd( MyServer *IRC_Server );
	int			PrivMsgCmd( MyServer *Irc_Server );
	int			NoticeCmd( MyServer *Irc_Server );
	int			JoinCmd( MyServer *IRC_Server );
	int			NamesCmd( MyServer *IRC_Server );
	int			ListCmd( MyServer *IRC_Server );
	int			InfoCmd( void );
	int			NamesCmd( MyServer *IRC_Server, MyMsg &msg );
	int			IsTheUserVisible( Channels *channel, Clients *sender );
	int			KickCmd( MyServer *IRC_Server );
	int			PartCmd( MyServer *IRC_Server );
	int			WallopsCmd( MyServer *IRC_Server );
	int			OperCmd( MyServer *IRC_Server );
	int			TopicCmd( MyServer *IRC_Server ); // MEV


	int			ValidateClientsConnections( void );


	/*TEST A EFFACER ENSUITE -- TESTER LE PARSING DE SMAUGDELA*/
	bool	parse_msg(void);
	bool 	Check_command(std::string str);
	void 	SetParams2(std::vector<std::string> params);


	/*======================*/
	std::string					Prefix;
	std::string					Command;
	std::vector<std::string>	Params;
private:
    MyMsg(/*ARG*/);

	/*RSI section 2.3.1 Message format in augmented BNF*/
	std::string 				_Message;
	//std::string					_Prefix;
	//std::string					_Command;
//	std::vector<std::string>	_Params;
	/*pour savoir de qui vient le message grace au socketfd*/
	Clients						*_SentFrom;
	int							_DoesCmdExist;
};

std::string toupper_striing( std::string cmd );


#endif