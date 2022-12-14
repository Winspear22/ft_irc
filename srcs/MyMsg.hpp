#ifndef MYMSG_HPP
# define MYMSG_HPP

# include "Clients.hpp"
# include "MyServer.hpp"

class Clients;
class MyServer;

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
	int			CheckFormatCmd( std::string cmd, std::vector<std::string> cmd_list );

	int			PassCmd( void );
	int			NickCmd( void );
	int			UserCmd( void );
	int			ModeCmd( void );
	int			PingCmd( void );
	int			ValidateClientsConnections( void );
	std::string					Prefix;
	std::string					Command;
	std::vector<std::string>	Params;
private:
    MyMsg(/*ARG*/);

	/*RSI section 2.3.1 Message format in augmented BNF*/
	std::string 				_Message;
	std::string					_Prefix;
	std::string					_Command;
	std::vector<std::string>	_Params;
	/*pour savoir de qui vient le message grace au socketfd*/
	Clients						*_SentFrom;
	int							_DoesCmdExist;
};

std::string toupper_striing( std::string cmd );


#endif