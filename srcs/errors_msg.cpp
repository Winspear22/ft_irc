# include "irc.hpp"
# include "MyServer.hpp"

int errors_handlers_msg( int error_code )
{
    if (error_code == ERROR_WRONG_PORT)
    {
		std::cout << RED << "Error. Wrong port used. Try a port between " << WHITE << "1024 " << RED << "and " << WHITE << "65535." << NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_SOCKET_CREATION)
    {
		std::cout << RED << "Error." << WHITE << " Socket(); " << RED << "had an error and returned -1. The socket was not created "<< NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_SOCKET_OPTIONS)
    {
		std::cout << RED<< "Error." << WHITE << " Setsockopt(); " << RED << "had an error and returned -1. Socket options were not chosen. "<< NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_SOCKET_BINDING)
    {
		std::cout << RED << "Error." << WHITE << " Bind(); " << RED << "had an error and returned -1. The socket was not bound."<< NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_LISTENING)
    {
		std::cout << RED << "Error." << WHITE << " Listen(); " << RED << "had an error and returned -1. No listening."<< NORMAL << std::endl;
		return (FAILURE);
	}
	if (error_code == ERROR_NONBLOCKING)
    {
		std::cout << RED << "Error." << WHITE << " Fcntl(); " << RED << "had an error and returned -1. The socket was not set to non-blocking status."<< NORMAL << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);

}


/*DANS LE WHILE, POUR EVITER D'AVOIR DES RETURN QUI FONT SORTIR DE LA BOUCLE
IL FAUR AVOIR DES FONCTIONS VOID POUR PAS AVOIR DE RETURN FAILURE QUI VONT NOUS
SORTIR DE LA BOUCLE*/
void loop_errors_handlers_msg( int error_code )
{
/*	if (error_code == ERROR_ACCEPT)
	{
		std::cout << PURPLE << "Waiting for connections..." << WHITE << " Accept(); " << PURPLE << "returned -1. Clients need to ask for connection." << NORMAL << std::endl;
		sleep(3);
		std::cout << YELLOW << ".";
		std::cout.flush();
		sleep(2);
		std::cout << WHITE << ".";
		std::cout.flush();
		sleep(2);
		std::cout << RED << ".";
		std::cout.flush();
		sleep(2);
		std::cout << WHITE << " Accept(); " << PURPLE << "returned -1. Clients need to ask for connection." << NORMAL << std::endl;
		sleep(2);
	}*/
	if (error_code == ERROR_ACCEPT)
		std::cout << RED << "Error." << WHITE << " Accept(); " << RED << "had an error and returned -1. No new client got connected to the server." << NORMAL << std::endl;
	if (error_code == ERROR_RECV)
		std::cout << RED << "Error." << WHITE << " Recv(); " << RED << "had an error and returned -1. Nothing was received." << NORMAL << std::endl;
	if (error_code == ERROR_SEND)
		std::cout << RED << "Error." << WHITE << " Send(); " << RED << "had an error and returned -1. Nothing was sent." << NORMAL << std::endl;
	if (error_code == ERROR_SELECT)
		std::cout << RED << "Error." << WHITE << " Select(); " << RED << "had an error and returned -1. No client was selected." << NORMAL << std::endl;
	
}

