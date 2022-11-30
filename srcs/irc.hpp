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

/*strlen*/
# include <string.h>

/*vector*/
# include <vector>

/*map*/
# include <map>

/*list*/
# include <list>

# include "MyServer.hpp"
# include "Clients.hpp"
/*===============================================================================================================*/
/*                                                DEFINES                                                        */
/*===============================================================================================================*/

/*                                             RETURN VALUES                                                     */

# define FAILURE -1
# define SUCCESS 0
# define EXIT_SUCCESS 0
# define EXIT_FAILIURE -1

/*                                             ERRORS CODES                                                      */

# define ERROR_SERVER -1
# define ERROR_WRONG_PORT 1
# define ERROR_SOCKET_CREATION 2
# define ERROR_SOCKET_OPTIONS 3
# define ERROR_SOCKET_BINDING 4
# define ERROR_LISTENING 5
# define ERROR_NONBLOCKING 6
# define ERROR_ACCEPT 7
# define ERROR_RECV 8
# define ERROR_SEND 9
# define ERROR_SELECT 10
# define ERROR_NEW 11
# define TIMEOUT 0
/*											   SERVER_STATUS													 */
# define SERVER_ON 0
# define SERVER_OFF 1

/*                                             TEXTE COLORS                                                      */

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define PURPLE "\033[1;35m"
# define CYAN "\033[1;36m"
# define WHITE "\033[1;37m"
# define NORMAL "\033[0m"

/*===============================================================================================================*/
/*                                                FUNCTIONS                                                      */
/*===============================================================================================================*/

/*                                              errors_handler.cpp                                                */
int     errors_handlers_msg( int error_code );
void    loop_errors_handlers_msg( int error_code );
//void	loop_safe_exit( Clients *sole_client, std::map<Clients*, int> _clients_list );

#endif
