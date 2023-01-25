#ifndef NUM_REPLIES_HPP
# define NUM_REPLIES_HPP

# include "MyMsg.hpp"
# include "Clients.hpp"
# include "Channels.hpp"
# include "MyServer.hpp"

class MyMsg;
class MyServer;
class Clients;
class Channels;


std::string RPL_WELCOME(MyMsg msg);
std::string RPL_YOURHOST(MyMsg msg);
std::string RPL_CREATED(MyMsg msg);
std::string RPL_MYINFO(MyMsg msg);
std::string ERR_NEEDMOREPARAMS(MyMsg msg);
std::string ERR_ALREADYREGISTRED(MyMsg msg);
std::string ERR_PASSWDMISMATCH();
std::string ERR_NONICKNAMEGIVEN();
std::string ERR_ERRONEUSNICKNAME(MyMsg msg);
std::string ERR_NICKNAMEINUSE(MyMsg msg, std::vector<std::string>::iterator it);
std::string ERR_UNAVAILRESOURCE(std::string tmp);
std::string ERR_RESTRICTED(MyMsg msg);
std::string RPL_UMODEIS(MyMsg msg);
std::string ERR_USERSDONTMATCH(MyMsg msg);
std::string ERR_UMODEUNKNOWNFLAG(MyMsg msg);
std::string ERR_NOPRIVILEGES(MyMsg msg);
std::string ERR_NOSUCHSERVER(MyMsg msg, std::vector<std::string>::iterator it);
std::string ERR_NOORIGIN(MyMsg msg);
std::string ERR_NOMOTD(MyMsg msg, int msg_type);
std::string RPL_MOTDSTART(MyMsg msg);
std::string RPL_MOTD(MyMsg msg, std::vector<std::string>::iterator it);
std::string RPL_ENDOFMOTD(MyMsg msg);
std::string ERR_NORECIPENT(MyMsg msg);
std::string ERR_NOTEXTTOSEND(MyMsg msg);
std::string ERR_NOSUCHNICK(MyMsg msg);
std::string RPL_VERSION(MyMsg msg);
std::string RPL_INFO(MyMsg msg, std::vector<std::string>::iterator it);
std::string RPL_ENDOFINFO(MyMsg msg);
std::string RPL_LIST(MyMsg msg, std::string name, std::string nb);
std::string RPL_LISTEND(MyMsg msg);
std::string ERR_NOSUCHCHANNEL(MyMsg msg, std::string it);
std::string ERR_NOTONCHANNEL(MyMsg msg, std::vector<std::string>::iterator it);
std::string ERR_NEEDMOREPARAMS2(MyMsg msg);
std::string RPL_YOUREOPER(MyMsg msg);
std::string ERR_NOTONCHANNEL(MyMsg msg, std::string chan_name);
std::string RPL_NOTOPIC( void );
std::string RPL_TOPIC(MyMsg msg, std::string chan, std::string topic);
std::string ERR_CHANOPRIVSNEEDED(MyMsg msg, std::map<Channels *, std::string>::iterator it);
std::string ERR_CHANOPRIVSNEEDED2(MyMsg msg, std::string it);
std::string ERR_NOTONCHANNEL(std::map<Channels *, std::string>::iterator it);
std::string ERR_USERONCHANNEL(MyMsg msg, std::string user_invited, std::string channel);
std::string ERR_CHANOPRIVSNEEDED(MyMsg msg, std::string channel);
std::string RPL_INVITING(MyMsg msg, std::string user_invited, std::string channel);
std::string ERR_USERNOTINCHANNEL(std::string client, std::string chan);
std::string RPL_NAMREPLY( MyMsg msg, std::string channames );
std::string RPL_ENDOFNAMES( MyMsg msg, std::string channames );
std::string RPL_TIME( MyMsg msg );
std::string ERR_TOOMANYCHANNELS( MyMsg msg, std::string channame );
std::string ERR_UNKNOWNMODE(MyMsg msg, std::string mode, std::string channame);
std::string RPL_CHANNELMODEIS(Channels *chan);

#endif