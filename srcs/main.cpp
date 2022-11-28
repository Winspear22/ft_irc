# include "irc.hpp"
# include "MyServer.hpp"

bool	ServerStatus;

void    signal_handler( int sig )
{
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
	return (SUCCESS);
}

int		check_serveur_creation(MyServer & Irc_Serveur)
{
	if (Irc_Serveur.CreateSocketFd() == ERROR_SOCKET_CREATION)
		return (errors_handlers_msg(ERROR_SOCKET_CREATION));
	if (Irc_Serveur.SetSocketOptions() == ERROR_SOCKET_OPTIONS)
		return (errors_handlers_msg(ERROR_SOCKET_OPTIONS));
	if (Irc_Serveur.BindSocketFd() == ERROR_SOCKET_BINDING)
		return (errors_handlers_msg(ERROR_SOCKET_BINDING));
	if (Irc_Serveur.ListenToSockedFd() == ERROR_LISTENING)
		return (errors_handlers_msg(ERROR_LISTENING));
	if (Irc_Serveur.SetSocketFdToNonBlocking() == ERROR_NONBLOCKING)
		return (errors_handlers_msg(ERROR_NONBLOCKING));
	return (SUCCESS);
}


/*static void	set_pollfd(Server *serv, std::vector<struct pollfd> &fds)
{
    std::map<sockfd, Client*>::const_iterator it
	fds.clear();

	if (serv == NULL)
		return ;
    it = serv->getUsers().begin();
	fds.push_back(pollfd());
	fds.back().fd = serv->getListener();
	fds.back().events = POLLIN;
	fds.back().revents = 0;

	while (it != serv->getUsers().end())
	{
		fds.push_back(pollfd());
		fds.back().fd = it->first;
		fds.back().events = POLLIN;
		fds.back().revents = 0;
		if (it->second->getLastcom() >= serv->getConfig()->getPing())
			it->second->send_to("PING " + it->second->getNickname());
        ++it;
	}
}

void	server_loop(Server *serv)
{
	std::vector<struct pollfd> fds;

	while (serv != NULL && server_running)
	{
		set_pollfd(serv, fds);

		poll(fds.data(), fds.size(), serv->getConfig()->getTimeout());
		for (size_t n = 0; n < fds.size(); n++)
		{
			if (fds[n].revents != 0)
			{
				if (fds[n].revents & POLLIN)
				{
					if (n == 0)		// The listening socket is at index 0.
						serv_accept(serv, fds);
					else if (n != 0)
						serv_receive(fds[n].fd, serv);
				}
				if (fds[n].revents & POLLHUP || fds[n].revents & POLLERR || fds[n].revents & POLLNVAL)
				{	
					std::cout << "Invalid event on socket #" << fds[n].fd << "." << std::endl;
					serv->getUser(fds[n].fd)->disconnect();
				}
			}
		}
		rm_deco_users(serv);
		rm_empty_chans(serv);
	}
}
*/

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
    while (ServerStatus != SERVER_OFF)
    {
		Irc_Serveur.AcceptClientsConnections();
	//	Irc_Serveur.RecvAndSend();
    }
    return (EXIT_SUCCESS);
}