#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "irc.hpp"

class Commands
{
public:
    Commands( void );
	Commands( const Commands & copy );
	Commands & operator=( const Commands & popo );
	~Commands( void );
private:
    //Commands(/*ARG*/);
};

#endif