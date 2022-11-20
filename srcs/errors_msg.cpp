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
		std::cout << RED << "Error." << WHITE << " fcntl(); " << RED << "had an error and returned -1. The socket was not set to non-blocking status."<< NORMAL << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);

}