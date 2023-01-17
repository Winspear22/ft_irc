# include "MyMsg.hpp"

// CONNECTION REGISTRATION

std::string RPL_WELCOME(MyMsg msg)
{
    std::string reply;

    reply = "001 " + msg.GetClients()->GetClientsNickname() + " : \033[1;33mWelcome to the ft_irc Network.\033[1;37m";

    return (reply);
}

std::string RPL_YOURHOST(MyMsg msg)
{
    std::string reply;

    reply = "002 " + msg.GetClients()->GetClientsNickname() + " : Your host is \033[1;31m localhost:6667.";

    return (reply);
}

std::string RPL_CREATED(MyMsg msg)
{
    std::string reply;
    time_t 		tmp;
	
	tmp = time(NULL);
    reply = "003 " + msg.GetClients()->GetClientsNickname() + " : This server was created \033[1;31m" + std::string(ctime(&tmp));
    return (reply);
}

std::string RPL_MYINFO(MyMsg msg)
{
    std::string reply;

    reply = "004 " + msg.GetClients()->GetClientsNickname() + " " + msg.GetClients()->GetServerName() + " 0.2";// SET <available umodes> <available cmodes> [<cmodes with param>]

    return (reply);
}

std::string ERR_NEEDMOREPARAMS(MyMsg msg)
{
    std::string reply;

    if (msg.GetClients()->GetClientsNickname().size() == 0)
        reply = "461 *" + msg.GetCmd() + ":Not enough parameters";
    else
        reply = "461 " + msg.GetClients()->GetClientsNickname() + msg.GetCmd() + ":Not enough parameters";
    return (reply);
}

std::string ERR_ALREADYREGISTRED(MyMsg msg)
{
    std::string reply;

    if (msg.GetClients()->GetClientsNickname().size() == 0)
        reply = "462 *" + msg.GetCmd() + ":Unauthorized command (already registered)";
    else
        reply = "462 " + msg.GetClients()->GetClientsNickname() + msg.GetCmd() + ":Unauthorized command (already registered)";
    return (reply);
}

std::string ERR_PASSWDMISMATCH()
{
    std::string reply;

    reply = "464 : Password incorrect";

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

    reply = "432 " + msg.GetClients()->GetClientsNickname() + ": Erroneous nickname";

    return (reply);
}
std::string ERR_NICKNAMEINUSE(MyMsg msg, std::vector<std::string>::iterator it)
{
    std::string reply;
    std::string content = *it;

    (void)msg;
    reply = "433 " + content + " " + content + " : Nickname is already in use";

    return (reply);
}

std::string ERR_UNAVAILRESOURCE(MyMsg msg)
{
    std::string reply;
    
    if (msg.GetCmd() == "NICK")
        reply = "437 " + msg.GetClients()->GetClientsNickname() + " : " + msg.GetClients()->GetClientsNickname() + "is temporarily unavailable";
    // if (msg.GetCmd() == "JOIN")
    //     reply = "437 " + msg.GetClients()->GetClientsNickname() + " :" + msg.getChannelName() + " is temporarily unavailable\n";
    return (reply);
}

std::string ERR_RESTRICTED(MyMsg msg)
{
    std::string reply;

    reply = "484 " + msg.GetClients()->GetClientsNickname() + ": Your connection is restricted!";

    return (reply);
}

std::string RPL_UMODEIS(MyMsg msg)
{
    std::string reply;

    reply = "221 " + msg.GetClients()->GetClientsNickname() + " " + msg.GetClients()->GetClientsMode(); // + "<user mode string>"

    return (reply);
}

std::string ERR_USERSDONTMATCH(MyMsg msg)
{
    std::string reply;

    reply = "502 " + msg.GetClients()->GetClientsNickname() + ": Cannot change mode for other users";

    return (reply);
}

std::string ERR_UMODEUNKNOWNFLAG(MyMsg msg)
{
    std::string reply;

    reply = "501 " + msg.GetClients()->GetClientsNickname() + " : Unknown MODE flag";

    return (reply);
}

std::string ERR_NOPRIVILEGES(MyMsg msg)
{
    std::string reply;

    reply = "481 " + msg.GetClients()->GetClientsNickname() + " : Permission Denied- You're not an IRC operator";

    return (reply);
}

std::string ERR_NOSUCHSERVER(MyMsg msg, std::vector<std::string>::iterator it)
{
    std::string reply;
    std::string iterator_content = *it;

    reply = "402 " + msg.GetClients()->GetClientsNickname() + ": " + iterator_content + ": No such server";

    return (reply);
}

std::string ERR_NOORIGIN(MyMsg msg)
{
    std::string reply;

    reply = "409 " + msg.GetClients()->GetClientsNickname() + " :No origin specified";
    return (reply);
}

std::string ERR_NOMOTD(MyMsg msg, int msg_type)
{
    std::string reply;

    if (msg_type == 1)
        reply = "422 " + msg.GetClients()->GetClientsNickname() + " :Too much parameters";
	else if (msg_type == 2)
		reply = "422 " + msg.GetClients()->GetClientsNickname() + " :MOTD File is missing";
	return (reply);
}

std::string RPL_MOTDSTART(MyMsg msg)
{
	std::string reply;

	reply = "375 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " " +  ":- \033[1;35m" + msg.GetClients()->GetServerName() + " \033[1;36mMessage of the day - \033[0m";
	return (reply);
}

std::string RPL_MOTD(MyMsg msg, std::vector<std::string>::iterator it)
{
	std::string reply;
    std::string iterator_content = *it;
	reply = "372 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " :- \033[1;34m" + iterator_content + "\033[0m";
	return (reply);
}

std::string RPL_ENDOFMOTD(MyMsg msg)
{
	std::string reply;

	reply = "376 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " \033[1;31m:End of MOTD command\033[0m";
	return (reply);
}

std::string ERR_NORECIPENT(MyMsg msg)
{
    std::string reply;

    reply = "411 \033[1;37m" + msg.GetClients()->GetClientsNickname() + "\033[1;31m :No recipent given PRIVMSG\033[0m";
    return (reply);
}

std::string ERR_NOTEXTTOSEND(MyMsg msg)
{
    std::string reply;

    reply = "412 \033[1;37m" + msg.GetClients()->GetClientsNickname() + "\033[1;31m :No text to send\033[0m";
    return (reply);
}

std::string ERR_NOSUCHNICK(MyMsg msg)
{
    std::string reply;

    reply = "401 \033[1;37m" + msg.GetClients()->GetClientsNickname() + "\033[1;31m :No such nick/channel\033[0m";
    return (reply);
}

void	 RPL_PRIVMSG(MyMsg *msg, std::string tmp, int version, MyServer * irc)
{
    std::string 	reply;
	int         	i;
    const char		*msg_sent;
    int         	msg_len;
    int         	ret_send;

    /*prefix + PRIVMSG + param[0] + " " + text*/
    std::cout << RED << "JE SUIS DANS RPL_PRIVMSG" << NORMAL << std::endl;
	i = 0;
	if (version == 0)
		reply = msg->GetPrefix() + " PRIVMSG " + msg->Params.at(0) + " " + tmp;
    else if (version == 1)
		reply = msg->GetPrefix() + " NOTICE " + msg->Params.at(0) + " " + tmp;
	msg_sent = reply.c_str();
	msg_len = strlen(msg_sent);
    while (i < msg_len)
    {
		std::cout << PURPLE << "msg == " << WHITE << &msg_sent[i] << NORMAL << std::endl;
        ret_send = send(irc->GetClientsThroughName(msg->Params.at(0))->GetClientsFd(), &msg_sent[i], msg_len - i, MSG_NOSIGNAL);
		if (ret_send == ERROR_SERVER)
			return (loop_errors_handlers_msg(ERROR_SEND));
        i = i + ret_send;
    }
    return ;
}

std::string RPL_VERSION(MyMsg msg)
{
	std::string reply;
	/*S'assurer de la bonne version dans le RPL*/
	reply = "351 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " \033[1;31mVersion 1.0 ft_irc\033[0m";
	return (reply);
}


std::string RPL_INFO(MyMsg msg, std::vector<std::string>::iterator it)
{
	std::string reply;
    std::string iterator_content = *it;
	/*S'assurer de la bonne version dans le RPL*/
	reply = "371 \033[1;37m" + msg.GetClients()->GetClientsNickname() + iterator_content;
	return (reply);
}

std::string RPL_ENDOFINFO(MyMsg msg)
{
	std::string reply;

	reply = "374 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " \033[1;31m:End of INFO list\033[0m";
	return (reply);
}

std::string RPL_LIST(MyMsg msg, std::string name, std::string nb)
{
	std::string reply;

	reply = "322 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " " + name + " " + nb;

    return (reply);
}

std::string RPL_LISTEND(MyMsg msg)
{
	std::string reply;

	reply = "323 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " \033[1;31m:End of list\033[0m";
	return (reply);
}

std::string ERR_NOSUCHCHANNEL(MyMsg msg, std::string it)
{
    std::string reply;

    reply = "403 " + msg.GetClients()->GetClientsNickname() + " " + it + " :No such channel.";
    return (reply);
}

std::string ERR_NOTONCHANNEL(MyMsg msg, std::vector<std::string>::iterator it)
{
    std::string reply;
    std::string iterator_content = *it;

    reply = "442 " + msg.GetClients()->GetClientsNickname() + " " + iterator_content + " :You're not on that channel";;

    return (reply);
}