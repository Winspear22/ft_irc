# include "num_replies.hpp"

/*=====================================*/
/*-----------REPLIES OF RFC------------*/
/*=====================================*/
std::string RPL_WELCOME(MyMsg msg)
{
    std::string reply;

    reply = "001 " + msg.GetClients()->GetClientsNickname() + " : Welcome to the \033[1;32mft_irc Network. \033[0m";

    return (reply);
}

std::string RPL_YOURHOST(MyMsg msg)
{
    std::string reply;

    reply = "002 " + msg.GetClients()->GetClientsNickname() + " : Your host is \033[1;36m" + msg.GetClients()->GetServerName() + "\033[0m";

    return (reply);
}

std::string RPL_CREATED(MyMsg msg)
{
    std::string reply;
    time_t 		tmp;
    std::string timestamp;
    std::string timestamp_2;
    size_t      pos;
	
	tmp = time(NULL);
    timestamp = std::string(ctime(&tmp));
    pos = timestamp.find("\n");
    timestamp_2 = timestamp.erase(pos);
    reply = "003 " + msg.GetClients()->GetClientsNickname() + " : This server was created \033[1;32m" + timestamp_2 + "\033[0m";
    return (reply);
}

std::string RPL_MYINFO(MyMsg msg)
{
    std::string reply;

    reply = "004 " + msg.GetClients()->GetClientsNickname() + " " + ": \033[1;36m" + msg.GetClients()->GetServerName() + " \033[1;32mv1\033[0m";// SET <available umodes> <available cmodes> [<cmodes with param>]
    reply += " Modes accessible : \033[1;35mi, w, o\033[0m";
    return (reply);
}

std::string RPL_UMODEIS(MyMsg msg)
{
    std::string reply;

    reply = "221 " + msg.GetClients()->GetClientsNickname() + " " + msg.GetClients()->GetClientsMode();

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

std::string RPL_VERSION(MyMsg msg)
{
	std::string reply;
	reply = "351 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " \033[1;31mVersion 1.0 ft_irc\033[0m";
	return (reply);
}


std::string RPL_INFO(MyMsg msg, std::vector<std::string>::iterator it)
{
	std::string reply;
    std::string iterator_content = *it;
	reply = "371 \033[1;37m" + msg.GetClients()->GetClientsNickname() + iterator_content;
	return (reply);
}

std::string RPL_ENDOFINFO(MyMsg msg)
{
	std::string reply;

	reply = "374 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " \033[1;31m:End of INFO list\033[0m";
	return (reply);
}

std::string RPL_LIST(MyMsg msg, std::string name, std::string topic)
{
	std::string reply;

	reply = "322 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " " + name + " :" + topic;
    return (reply);
}

std::string RPL_LISTEND(MyMsg msg)
{
	std::string reply;

	reply = "323 \033[1;37m" + msg.GetClients()->GetClientsNickname() + " \033[1;31m:End of list\033[0m";
	return (reply);
}

std::string RPL_YOUREOPER(MyMsg msg)
{
    std::string reply;

    reply = "381 " + msg.GetClients()->GetClientsNickname() + " " + ":You are now an IRC operator";
    return (reply);
}

std::string RPL_NOTOPIC( void )
{
    std::string reply;

	reply = "331 : No topic is set";
    return (reply);
}

std::string RPL_TOPIC(MyMsg msg, std::string chan, std::string topic)
{
    std::string reply;

    reply = "332 " + msg.GetClients()->GetClientsNickname() + " " + chan + " :" + topic;
    return (reply);
}

std::string RPL_INVITING(MyMsg msg, std::string user_invited, std::string channel)
{
    std::string reply;

    reply = "341 " + msg.GetClients()->GetClientsNickname() + " " + channel + + " " + user_invited;
    return (reply);
}

std::string RPL_NAMREPLY( MyMsg msg, std::string channames )
{
    std::string reply;

    reply = "353 " + msg.GetClients()->GetClientsNickname() + " = " + channames + " :";
    return (reply);
}

std::string RPL_ENDOFNAMES( MyMsg msg, std::string channames )
{
    std::string reply;

    reply = "366 " + msg.GetClients()->GetClientsNickname() + " " + channames + " :" + "End of /NAMES list";
    return (reply);
}

std::string RPL_TIME( MyMsg msg )
{
    std::string reply;
    time_t 		tmp;
    std::string timestamp;
    std::string timestamp_2;
    size_t      pos;
	
	tmp = time(NULL);
    timestamp = std::string(ctime(&tmp));
    pos = timestamp.find("\n");
    timestamp_2 = timestamp.erase(pos);

    reply = "391 " + msg.GetClients()->GetServerName() + " :\033[1;34m" + timestamp_2 + "\033[0m";
    return (reply);
}


std::string RPL_CHANNELMODEIS(Channels *chan)
{
    std::string reply;
    std::map<Clients *, int>::iterator it;

    it = chan->GetAllowedToInviteList().begin();

    reply = "324 " + chan->GetChannelName() + " " + chan->GetChannelMode() + " ";
    while (it != chan->GetAllowedToInviteList().end())
    {
        reply += it->first->GetClientsNickname() + " ";
        it++;
    }
    return (reply);
}

/*=====================================*/
/*------------END OF REPLIES-----------*/
/*=====================================*/


/*=====================================*/
/*-------------ERRORS OF RFC-----------*/
/*=====================================*/


std::string ERR_NEEDMOREPARAMS(MyMsg msg)
{
    std::string reply;

    reply = "461 " + msg.GetCmd() + " " + ":Not enough parameters";
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

std::string ERR_UNAVAILRESOURCE(std::string nick)
{
    std::string reply;
    
    reply = "437 " + nick + " " + nick;

    return (reply);
}

std::string ERR_UNKNOWNMODE(MyMsg msg, std::string mode, std::string channame)
{
    (void)msg;
    std::string reply;

    reply = "472 " + mode + " :is unknown mode char to me for " + channame;

    return (reply);
}

std::string ERR_RESTRICTED(MyMsg msg)
{
    std::string reply;

    reply = "484 " + msg.GetClients()->GetClientsNickname() + ": Your connection is restricted!";

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
    (void)msg;

    reply = "402 :" + iterator_content + ": No such server";

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

std::string ERR_NORECIPENT(MyMsg msg)
{
    std::string reply;

    reply = "411 " + msg.GetClients()->GetClientsNickname() + " :No recipent given PRIVMSG";
    return (reply);
}

std::string ERR_NOTEXTTOSEND(MyMsg msg)
{
    std::string reply;

    reply = "412 " + msg.GetClients()->GetClientsNickname() + " :No text to send";
    return (reply);
}

std::string ERR_NOSUCHNICK(MyMsg msg)
{
    std::string reply;

    reply = "401 " + msg.GetClients()->GetClientsNickname() + " :No such nick/channel";
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

    reply = "442 " + msg.GetClients()->GetClientsNickname() + " " + iterator_content + ":You're not on that channel";

    return (reply);
}

std::string ERR_NEEDMOREPARAMS2(MyMsg msg)
{
    std::string reply;

    reply = "461 " + msg.GetClients()->GetClientsNickname() + " " + ":Not enough parameters";
    return (reply);
}

std::string ERR_NOTONCHANNEL(MyMsg msg, std::string chan_name)
{
    std::string reply;

    reply = "442 " + msg.GetClients()->GetClientsNickname() + " " + chan_name + " :" + "You're not on that channel"; 
    return (reply);
}

std::string ERR_CHANOPRIVSNEEDED(MyMsg msg, std::map<Channels *, std::string>::iterator it)
{
    std::string reply;

    reply = "482 " + msg.GetClients()->GetClientsNickname() + " " + it->second + " :" + "You're not channel operator";
    return (reply); 
}

std::string ERR_CHANOPRIVSNEEDED2(MyMsg msg, std::string it)
{
    std::string reply;

    reply = "482 " + msg.GetClients()->GetClientsNickname() + " " + it + " :" + "You're not channel operator";
    return (reply); 
}

std::string ERR_NOTONCHANNEL(std::map<Channels *, std::string>::iterator it)
{
    std::string reply;

    reply = "442 " + it->second + " :" + "You're not on that channel";
    return (reply);
}

std::string ERR_USERONCHANNEL(MyMsg msg, std::string user_invited, std::string channel)
{
    std::string reply;

    reply = "443 " + msg.GetClients()->GetClientsNickname() + " " + user_invited + + " " + channel + " :is already on channel";
    return (reply);
}

std::string ERR_CHANOPRIVSNEEDED(MyMsg msg, std::string channel)
{
    std::string reply;

    reply = "482 " + msg.GetClients()->GetClientsNickname() + " " + channel + " :You're not channel operator";

    return (reply);
}

std::string ERR_USERNOTINCHANNEL(std::string client, std::string chan)
{
    std::string reply;

    reply = "441 " + client + " " + chan + " :" + "They aren't on that channel";
    return (reply);
}

std::string ERR_TOOMANYCHANNELS(MyMsg msg, std::string channame)
{
    std::string reply;

    reply = "405 " + msg.GetClients()->GetClientsNickname() + " " + channame + " :You have joined too much channels";

    return (reply);
}

/*=====================================*/
/*------------END OF ERRORS------------*/
/*=====================================*/