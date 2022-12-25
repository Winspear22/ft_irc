# include "MyMsg.hpp"

// CONNECTION REGISTRATION

std::string RPL_WELCOME(MyMsg msg)
{
    std::string reply;

    reply = "001 " + msg.GetClients()->GetClientsNickname() + ": \033[1;33mWelcome to the ft_irc Network.\033[1;37m\n";

    return (reply);
}

std::string RPL_YOURHOST(MyMsg msg)
{
    std::string reply;

    reply = "002 " + msg.GetClients()->GetClientsNickname() + ": Your host is \033[1;31m localhost:6667.\n";

    return (reply);
}

std::string RPL_CREATED(MyMsg msg)
{
    std::string reply;
    time_t 		tmp;
	
	tmp = time(NULL);
    reply = "003 " + msg.GetClients()->GetClientsNickname() + ": This server was created \033[1;31m" + std::string(ctime(&tmp));
    return (reply);
}

std::string RPL_MYINFO(MyMsg msg)
{
    std::string reply;

    reply = "004 " + msg.GetClients()->GetClientsNickname(); // SET <available umodes> <available cmodes> [<cmodes with param>]
    return (reply);
}

std::string ERR_NEEDMOREPARAMS(MyMsg msg)
{
    std::string reply;

    if (msg.GetClients()->GetClientsNickname().size() == 0)
        reply = "461 *" + msg.GetCmd() + ":Not enough parameters\n";
    else
        reply = "461 " + msg.GetClients()->GetClientsNickname() + msg.GetCmd() + ":Not enough parameters\n";
    return (reply);
}

std::string ERR_ALREADYREGISTRED(MyMsg msg)
{
    std::string reply;

    if (msg.GetClients()->GetClientsNickname().size() == 0)
        reply = "462 *" + msg.GetCmd() + ":Unauthorized command (already registered)\n";
    else
        reply = "462 " + msg.GetClients()->GetClientsNickname() + msg.GetCmd() + ":Unauthorized command (already registered)\n";
    return (reply);
}

std::string ERR_PASSWDMISMATCH()
{
    std::string reply;

    reply = "464 : Password incorrect\n";

    return (reply);
}

std::string ERR_NONICKNAMEGIVEN()
{
    std::string reply;

    reply = "431 : No nickname given";

    return (reply);
}

std::string ERR_ERRONEUSNICKNAME(MyMsg msg)
{
    std::string reply;

    reply = "432 " + msg.GetClients()->GetClientsNickname() + ": Erroneous nickname\n";

    return (reply);
}
std::string ERR_NICKNAMEINUSE(MyMsg msg)
{
    std::string reply;

    reply = "433 " + msg.GetClients()->GetClientsNickname() + ": Nickname is already in use\n";

    return (reply);
}

std::string ERR_UNAVAILRESOURCE(MyMsg msg)
{
    std::string reply;
    
    if (msg.GetCmd() == "NICK")
        reply = "437 " + msg.GetClients()->GetClientsNickname() + " : " + msg.GetClients()->GetClientsNickname() + "is temporarily unavailable\n";
    // if (msg.GetCmd() == "JOIN")
    //     reply = "437 " + msg.GetClients()->GetClientsNickname() + " :" + msg.getChannelName() + " is temporarily unavailable\n";
    return (reply);
}

std::string ERR_RESTRICTED(MyMsg msg)
{
    std::string reply;

    reply = "484 " + msg.GetClients()->GetClientsNickname() + ": Your connection is restricted!\n";

    return (reply);
}

std::string RPL_UMODEIS(MyMsg msg)
{
    std::string reply;

    reply = "221 " + msg.GetClients()->GetClientsNickname(); // + "<user mode string>"

    return (reply);
}

std::string ERR_USERSDONTMATCH(MyMsg msg)
{
    std::string reply;

    reply = "502 " + msg.GetClients()->GetClientsNickname() + ": Cannot change mode for other users\n";

    return (reply);
}

std::string ERR_UMODEUNKNOWNFLAG(MyMsg msg)
{
    std::string reply;

    reply = "501 " + msg.GetClients()->GetClientsNickname() + " : Unknown MODE flag\n";

    return (reply);
}

std::string ERR_NOPRIVILEGES(MyMsg msg)
{
    std::string reply;

    reply = "481 " + msg.GetClients()->GetClientsNickname() + " : Permission Denied- You're not an IRC operator";

    return (reply);
}

std::string ERR_NOSUCHSERVER(MyMsg msg)
{
    std::string reply;

    reply = "402 " + msg.GetClients()->GetClientsNickname() + " servername : No such channel";

    return (reply);
}
