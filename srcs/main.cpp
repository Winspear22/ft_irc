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
		Irc_Serveur.RecvAndSend();
    }
    return (EXIT_SUCCESS);
}

/*#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char hello[] = "Hello from server";
 
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
	{
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
	else 
		std::cout << GREEN << "Accept success !" << NORMAL << std::endl;
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
 
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    (void)valread;
    return 0;
}
*/
