#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include "irc.hpp"

class Channels
{
public:
    Channels( void );
	Channels( const Channels & copy );
	Channels & operator=( const Channels & popo );
	~Channels( void );
private:
    Channels(/*ARG*/);
};

#endif