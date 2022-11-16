# include "irc.hpp"
# include "MyServer.hpp"
int main(int argc, char **argv)
{
    MyServer Irc_Server;
(void)argv;
    if (argc != 3)
    {
        std::cout << RED << "Error. Wrong number of arguments. You need " << WHITE << "3 " << RED << "you have " << WHITE << argc << RED << "." << std::endl;
        std::cout << RED << "Use the following configuration : " << WHITE << "./ircserv <port> <password>" << NORMAL << std::endl;
        return (FAILURE);
    }
    else
    {
        Irc_Server.SetPort(argv[1]);
        Irc_Server.SetPassword(argv[2]);
        std::cout << Irc_Server << std::endl;
        /*std::cout << Irc_Server.GetPassword() << std::endl;
        std::cout << Irc_Server.GetPort() << std::endl;*/
    }
    return (0);
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
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
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

