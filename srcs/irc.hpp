#ifndef IRC_HPP
# define IRC_HPP
/*===============================================================================================================*/
                                                /*INCLUDES*/
/*===============================================================================================================*/
# include <iomanip>
# include <iostream>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <csignal>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <poll.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <vector>
# include <map>
# include <list>
# include <ctime>
# include <algorithm>
# include <fstream>
# include "MyServer.hpp"
# include "Clients.hpp"

/*===============================================================================================================*/
/*                                                DEFINES                                                        */
/*===============================================================================================================*/

/*                                              OPERATOR ID                                                      */
# define PASSWORD_OF_OPERATORS "1234"
# define ID_OF_OPERATORS      "oper"  

/*                                             RETURN VALUES                                                     */

# define FAILURE -1
# define SUCCESS 0
# define EXIT_SUCCESS 0
# define EXIT_FAILIURE -1
# define NO 1
# define YES 0
# define DELAY 2 * CLOCKS_PER_SEC

/*                                             ERRORS CODES                                                      */

# define ERROR_SERVER -1
# define ERROR_WRONG_PORT 1
# define ERROR_NOT_PORT_6667 12
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
# define ERROR_CONFIG 12
# define ERROR_TOOMUCHCLIENTS 13
# define TIMEOUT 0
# define CMD_EXISTS 0
# define CMD_DOESNT_EXIST 1
# define ERROR_USER_DISCONNECTED 0


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

#endif
