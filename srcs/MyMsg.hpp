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
private:
    MyMsg(/*ARG*/);

	/*RSI section 2.3.1 Message format in augmented BNF*/
	std::string _Message;
	std::string	_Prefix;
	std::string	_Command;
	std::string	_Params;
};

#endif