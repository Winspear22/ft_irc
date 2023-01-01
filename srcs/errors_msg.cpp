# include "irc.hpp"
# include "MyServer.hpp"
# include "Clients.hpp"

int errors_handlers_msg( int error_code )
{
    if (error_code == ERROR_WRONG_PORT)
    {
		std::cerr << RED << "Error. Wrong port used. You cannot use a port under " << WHITE << "1024 " << RED << "and above " << WHITE << "65535" << RED \
		<< ". Moreover, the only authorized port for this server is " << WHITE << "6667" << RED << "." << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_NOT_PORT_6667)
    {
		std::cerr << RED << "Error. Wrong port used. You must use the port " << WHITE << "6667" << RED << "." << NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_SOCKET_CREATION)
    {
		std::cerr << RED << "Error." << WHITE << " Socket(); " << RED << "had an error and returned -1. The socket was not created "<< NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_SOCKET_OPTIONS)
    {
		std::cerr << RED<< "Error." << WHITE << " Setsockopt(); " << RED << "had an error and returned -1. Socket options were not chosen. "<< NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_SOCKET_BINDING)
    {
		std::cerr << RED << "Error." << WHITE << " Bind(); " << RED << "had an error and returned -1. The socket was not bound."<< NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_LISTENING)
    {
		std::cerr << RED << "Error." << WHITE << " Listen(); " << RED << "had an error and returned -1. No listening."<< NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_NONBLOCKING)
    {
		std::cerr << RED << "Error." << WHITE << " Fcntl(); " << RED << "had an error and returned -1. The socket was not set to non-blocking status."<< NORMAL << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);

}

/*DANS LE WHILE, POUR EVITER D'AVOIR DES RETURN QUI FONT SORTIR DE LA BOUCLE
IL FAUR AVOIR DES FONCTIONS VOID POUR PAS AVOIR DE RETURN FAILURE QUI VONT NOUS
SORTIR DE LA BOUCLE*/
void loop_errors_handlers_msg( int error_code )
{
	if (error_code == ERROR_ACCEPT)
		std::cerr << RED << "Error." << WHITE << " Accept(); " << RED << "had an error and returned -1. No connection was accepted." << NORMAL << std::endl;
	if (error_code == ERROR_SEND)
		std::cerr << RED << "Error." << WHITE << " Send(); " << RED << "had an error and returned -1. Nothing was sent." << NORMAL << std::endl;
	if (error_code == ERROR_SELECT)
		std::cerr << RED << "Error." << WHITE << " Select(); " << RED << "had an error and returned -1. No client was selected." << NORMAL << std::endl;
	if (error_code == ERROR_NONBLOCKING)
		std::cerr << RED << "Error." << WHITE << " Fcntl(); " << RED << "had an error and returned -1. The client created fd is not set to non-blocking option." << NORMAL << std::endl;
//	if (error_code == TIMEOUT)
//		std::cerr << RED << "Error." << WHITE << " Select(); " << RED << "has timeout and exceeded the 2 minutes delay."<< NORMAL << std::endl;
}