#ifndef MYMSG_HPP
# define MYMSG_HPP

# include "Clients.hpp"

class Clients;

class MyMsg
{
public:
   /* MyMsg( Clients *popo, std::string MyMsg );
    MyMsg( const MyMsg & copy );
	MyMsg & operator=( const MyMsg & rhs );
	~MyMsg( void );*/

    MyMsg( Clients  *popo, std::string lolo );
	MyMsg( const MyMsg & copy );
	MyMsg & operator=( const MyMsg & popo );
	~MyMsg( void );
private:
    MyMsg(/*ARG*/);
};

#endif