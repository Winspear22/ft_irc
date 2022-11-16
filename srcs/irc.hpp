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

/*atoi*/
# include <stdlib.h>


/*===============================================================================================================*/
/*                                                DEFINES                                                        */
/*===============================================================================================================*/

/*                                             RETURN VALUES                                                     */

# define FAILURE -1
# define SUCCESS 0

/*                                             TEXTE COLORS                                                      */

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define PURPLE "\033[1;35m"
# define CYAN "\033[1;36m"
# define WHITE "\033[1;37m"
# define NORMAL "\033[0m"
#endif