#ifndef IRC_HPP
# define IRC_HPP
/*===============================================================================================================*/
                                                /*INCLUDES*/
/*===============================================================================================================*/
/*Classic libraries to implement c++ features*/
# include <iomanip>
# include <iostream>
/*socket - setsockopt - getsockname - getprotobyname - gethostbyname - getaddrinfo - freeaddrinfo - bind
- connect - listen - accept - recv - */
# include <sys/socket.h>
# include <netdb.h>
/*htons - htonl - ntohs - ntohl - inet_addr - inet_ntoa*/
# include <arpa/inet.h>
/*signal library*/
# include <csignal>
/*lseek*/
# include <sys/types.h>
/*fstat*/
# include <sys/stat.h>
/*fnctl*/
# include <fcntl.h>
/*poll*/
# include <poll.h>

#endif