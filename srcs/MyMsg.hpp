#ifndef MYMSG_HPP
# define MYMSG_HPP

# include "Clients.hpp"

class Clients;

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
	std::string GetParams( void );
	void 		SetPrefix( std::string );
	void 		SetCmd( std::string );
	void 		SetParams( std::string );
	int			CheckFormatCmd( std::string cmd, std::vector<std::string> cmd_list );

private:
    MyMsg(/*ARG*/);

	/*RSI section 2.3.1 Message format in augmented BNF*/
	std::string _Message;
	std::string	_Prefix;
	std::string	_Command;
	std::string	_Params;
	/*pour savoir de qui vient le message grace au socketfd*/
	Clients		*_SentFrom;
};

std::string toupper_striing( std::string cmd );


#endif