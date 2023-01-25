# include "irc.hpp"
# include "MyServer.hpp"

bool	ServerStatus;

void    signal_handler( int sig )
{
	if (sig == SIGTERM)
		std::cout << RED << "Fermeture de client détectée" << NORMAL << std::endl;
	else if (sig == SIGPIPE)
		std::cout << RED << "Fermeture de netcat détectée" << NORMAL << std::endl;
	std::cout << std::endl;
    std::cout << GREEN << "Signal button pressed : " << YELLOW << "sig n° : " 
	<< sig << WHITE << " - SIGINT." << NORMAL << std::endl;
	ServerStatus = SERVER_OFF;
}

int		check_serveur_args(MyServer & Irc_Serveur)
{
	if (Irc_Serveur.GetPort() < 1024)
		return (errors_handlers_msg(ERROR_WRONG_PORT));
	else if (Irc_Serveur.GetPort() > 65535)
		return (errors_handlers_msg(ERROR_WRONG_PORT));
	else if (Irc_Serveur.GetPort() != 6667)
		return (errors_handlers_msg(ERROR_NOT_PORT_6667));
	return (SUCCESS);
}

int		check_serveur_creation(MyServer & Irc_Serveur)
{
	if (Irc_Serveur.CreateSocketFd() == ERROR_SOCKET_CREATION)
		return (errors_handlers_msg(ERROR_SOCKET_CREATION));
	if (Irc_Serveur.SetSocketOptions() == ERROR_SOCKET_OPTIONS)
		return (errors_handlers_msg(ERROR_SOCKET_OPTIONS));
	if (Irc_Serveur.SetSocketFdToNonBlocking(Irc_Serveur.GetSocketFd()) == ERROR_NONBLOCKING)
		return (errors_handlers_msg(ERROR_NONBLOCKING));
	if (Irc_Serveur.BindSocketFd() == ERROR_SOCKET_BINDING)
		return (errors_handlers_msg(ERROR_SOCKET_BINDING));
	if (Irc_Serveur.ListenToSockedFd() == ERROR_LISTENING)
		return (errors_handlers_msg(ERROR_LISTENING));
	return (SUCCESS);
}

void	WelcomeOnTheServer( void )
{
	std::cout << "\033[2J" << std::flush;
	std::cout << BLUE << "==============================================================================================================================" << NORMAL << std::endl;
	std::cout << YELLOW << "| |  | |    | |                          | |                           |_   _| ___ \\/  __ \\ /  ___|                         " << NORMAL << std::endl;
	std::cout << YELLOW << "| |  | | ___| | ___ ___  _ __ ___   ___  | |_ ___     ___  _   _ _ __    | | | |_/ /| /  \\/ \\ `--.  ___ _ ____   _____ _ __ " << NORMAL << std::endl;
	std::cout << YELLOW << "| |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\   / _ \\| | | | '__|   | | |    / | |      `--. \\/ _ \\ '__\\ \\ / / _ \\ '__|" << NORMAL << std::endl;
	std::cout << YELLOW << "\\  /\\  /  __/ | (_| (_) | | | | | |  __/ | || (_) | | (_) | |_| | |     _| |_| |\\ \\ | \\__/\\ /\\__/ /  __/ |   \\ V /  __/ |   " << NORMAL << std::endl;
	std::cout << YELLOW << " \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/   \\___/ \\__,_|_|     \\___/\\_| \\_| \\____/ \\____/ \\___|_|    \\_/ \\___|_|   " << NORMAL << std::endl;
	std::cout << BLUE << "==============================================================================================================================" << NORMAL << std::endl;
	std::cout << WHITE << "Team :" << NORMAL << std::endl;
	std::cout << RED << "Malbrand" << NORMAL << std::endl;
	std::cout << BLUE << "Naben-za" << NORMAL << std::endl;
	std::cout << YELLOW "Adaloui" << NORMAL << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << RED << "Error. Wrong number of arguments. You need " << WHITE << "3 " << RED << "you have " << WHITE << argc << RED << "." << std::endl;
        std::cout << RED << "Use the following configuration : " << WHITE << "./ircserv <port> <password>" << NORMAL << std::endl;
        return (EXIT_FAILIURE);
    }
    MyServer Irc_Serveur(atoi(argv[1]), argv[2]);
	ServerStatus = SERVER_ON;
	signal(SIGINT, signal_handler);
	if (check_serveur_args(Irc_Serveur) == FAILURE)
		return (FAILURE);
	if (check_serveur_creation(Irc_Serveur) == FAILURE)
		return (FAILURE);
	Irc_Serveur.InitVariables();
	WelcomeOnTheServer();
	while (ServerStatus != SERVER_OFF)
		Irc_Serveur.SelectClients();
    return (EXIT_SUCCESS);
}