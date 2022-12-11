# include "MyMsg.hpp"
# include <sys/socket.h>
# include <sys/types.h>

MyMsg::MyMsg( void )
{
	//std::cout << RED << "Wrong constructor used." << NORMAL << std::endl;
	return ;
}


MyMsg::MyMsg( Clients *client, std::string MyMsg )
{
	this->_Message = MyMsg;
	this->_SentFrom = client;
	//std::cout << GREEN << "MyMsg Constructor called." << NORMAL << std::endl;
	return ;
}

MyMsg::MyMsg( const MyMsg & copy )
{
	//std::cout << "\033[0;33mMyMsg Copy Constructor called." << NORMAL << std::endl;
	*this = copy;
    return ;
}

MyMsg & MyMsg::operator=( MyMsg const & rhs )
{
	//std::cout << "\033[0;34mMyMsg Copy assignment operator called." << NORMAL << std::endl;
	this->_Message = rhs._Message;
	this->_SentFrom = rhs._SentFrom;
	return (*this);
}


MyMsg::~MyMsg( void )
{
	//std::cout << RED << "MyMsg Destructor called." << NORMAL << std::endl;
	return ;
}

std::string MyMsg::GetMsg( void )
{
	return (this->_Message);
}

std::string MyMsg::GetPrefix( void )
{
	return (this->_Prefix);
}

std::string MyMsg::GetCmd( void )
{
	return (this->_Command);
}

/*std::string MyMsg::GetParams( void )
{
	return (this->_Params);
}*/

Clients	*MyMsg::GetClients( void )
{
	return (this->_SentFrom);
}

void MyMsg::SetPrefix( std::string Prefix)
{
	this->_Prefix = Prefix;
}

std::string toupper_striing( std::string cmd )
{
	int i;
	std::string capitalized_str;

	i = -1;
	while (cmd[++i])
		capitalized_str[i] = toupper(cmd[i]);
	return (capitalized_str);
}

void MyMsg::SetCmd( std::string Cmd )
{
	this->_Command = toupper_striing( Cmd );
}

void MyMsg::SetParams( std::string Params )
{
	this->_Params.push_back(Params);
}

int			MyMsg::CheckFormatCmd( std::string cmd, std::vector<std::string> cmd_list )
{
	/*int i;

	i = 0;
	if (cmd.size() != 3)
		return (FAILURE);
	if (!isalpha(cmd[0]))
		return (FAILURE);
	while (cmd[++i])
	{
		if (!isdigit(cmd[i]))
			return (FAILURE);
	}*/
	std::vector<std::string>::iterator it;
	
	it = cmd_list.begin();
	while (it != cmd_list.end())
	{
		if (*it == cmd)
			return (SUCCESS);
		it++;
	}
	
	return (FAILURE);
}

/*LA COMMANDE PASS QUI VERIFIE LA VERACITE DU PASS*/
int		MyMsg::PassCmd( void )
{
	std::string str;
	//std::cout << RED << "PARAMS = " << this->_Params << NORMAL << std::endl;
	
	if (this->_Params.size() >= 1)
	{
		str = "\033[1;31mERR_NEEDMOREPARAMS \033[1;37mPASS :Not enough parameters\n\033[0m";
		//send(this->_SentFrom->GetClientsFd(), str.c_str(), strlen(str.c_str()), MSG_DONTWAIT);
		SendMsgBackToClients(*this, str);
	}
	return (SUCCESS);
}

/*LA COMMANDE NICK QUI INITIALISE LE NICNAME*/

int	MyMsg::NickCmd( void )
{
	std::string str;

	this->_SentFrom->SetClientsNickname(this->_Params.front());
	//std::cout << RED << "PARAMS = " << this->_Params << NORMAL << std::endl;

	if (this->_Params.size() >= 1)
	{
		str = "\033[1;35mIntroducing new nick \033[1;37m" + this->_SentFrom->GetClientsNickname() + "\n";
		SendMsgBackToClients(*this, str);
	}
	return (SUCCESS);
}

/*LA COMMANDE NICK QUI SET LE USERNAME, LE HOSTNAME ET LE REALNAME 
ELLE EST FAIE A LA ZOB MAIS ELLE MARCHE, C'EST L'IMPORTANT */
/*TROUVER COMMENT CHANGER LE USERNAME, HOSTNAME ET REALNAME DANS LES 
OPTIONS POUR FAIRE UN VRAI ALGORITHME*/
int	MyMsg::UserCmd( void )
{
	std::string msg_sent;
	std::string username;
	std::string hostname;
	std::string realname;
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator test;

	it = this->_Params.begin();
	test = this->_Params.begin();
	while (test != this->_Params.end())
	{
		std::cout << CYAN << "test == " << WHITE << *test << NORMAL << std::endl;
		test++;
	}
	username = *it;
	this->_SentFrom->SetClientsUsername(username);
	it++;
	hostname = *it;
	it++;
	hostname += " " + *it;
	this->_SentFrom->SetClientsHostname(hostname);
	it++;
	realname = *it;
	it++;
	if (it != this->_Params.end())
	{
		while (it != this->_Params.end())
		{
			realname += " " + *it;
			it++;
		}
	}
	this->_SentFrom->SetClientsRealname(realname);
	std::cout << RED << "Usernane = " << WHITE << this->_SentFrom->GetClientsUsername() << NORMAL << std::endl;
	std::cout << RED << "Hostname = " << WHITE << this->_SentFrom->GetClientsHostname() << NORMAL << std::endl;
	std::cout << RED << "Realname = " << WHITE << this->_SentFrom->GetClientsRealname() << NORMAL << std::endl;
	std::cout << RED << "Nickname = " << WHITE << this->_SentFrom->GetClientsNickname() << NORMAL << std::endl;
	return (SUCCESS);
}

#include <ctime>

int		MyMsg::ValidateClientsConnections( void )
{
	std::string RPL_WELCOME;
	std::string RPL_YOURHOST;
	std::string RPL_CREATED;
	std::string RPL_MYINFO;
	time_t 		tmp;
	
	tmp = time(NULL);
	RPL_WELCOME = "001 " + this->_SentFrom->GetClientsNickname() + " \033[1;33mWelcome to the Internet Relay Network \033[1;37m" + this->_SentFrom->GetClientsNickname() + "!" + this->_SentFrom->GetClientsUsername() + "@" + this->_SentFrom->GetClientsHostname();
	RPL_YOURHOST = "002 " + this->_SentFrom->GetClientsNickname() + "\033[1;33m Your host is \033[1;31m" + "MyServerName" + "\033[1;33m, running version \033[1;31m" + "0.2";
	RPL_CREATED = "003 " + this->_SentFrom->GetClientsNickname() + "\033[1;33m This server was created \033[1;37m" + std::string(ctime(&tmp)); //"dimanche 21 mai (UTC+0200) at 2017, 09:08:01";
	RPL_MYINFO = "004 " + this->_SentFrom->GetClientsNickname() + " " + "MyServerName" + " " + "0.2AAAAAAAAAAAAAAAAAAAAA";
	SendMsgBackToClients(*this, RPL_WELCOME);
	SendMsgBackToClients(*this, RPL_YOURHOST);
	SendMsgBackToClients(*this, RPL_CREATED);
	SendMsgBackToClients(*this, RPL_MYINFO);
	SendMsgBackToClients(*this, "\r\n");
	this->_SentFrom->SetClientsConnectionPermission(YES);
	return (SUCCESS);
}