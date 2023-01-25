# include "MyMsg.hpp"
# include "MyServer.hpp"
# include "Channels.hpp"
# include "num_replies.hpp"
# include "Clients.hpp"
# include <sys/socket.h>
# include <sys/types.h>

MyMsg::MyMsg( void )
{
	return ;
}

MyMsg::MyMsg( Clients *client, std::string MyMsg )
{
	this->_Message = MyMsg;
	this->_SentFrom = client;
	return ;
}

MyMsg::MyMsg( const MyMsg & copy )
{
	*this = copy;
    return ;
}

MyMsg & MyMsg::operator=( MyMsg const & rhs )
{
	this->_Message = rhs._Message;
	this->_SentFrom = rhs._SentFrom;
	return (*this);
}

MyMsg::~MyMsg( void )
{
	return ;
}

	/*===========================================*/
	/*             GETTERS AND SETTERS           */
	/*--------------All the Setters--------------*/

void 					MyMsg::SetPrefix( std::string Prefix)
{
	this->Prefix = Prefix;
}

void 					MyMsg::SetCmd( std::string Cmd )
{
	this->Command = toupper_striing( Cmd );
}

void					MyMsg::SetCmdExistence( int CmdStatus )
{
	this->_DoesCmdExist = CmdStatus;
}

void 					MyMsg::SetParams( std::string Params )
{
	this->Params.push_back(Params);
}

void 					MyMsg::SetParams2(std::vector<std::string> params)
{
	this->Params = params;
}

	/*-------------------------------------------*/
	/*--------------All the Getters--------------*/

std::string 			MyMsg::GetMsg( void )
{
	return (this->_Message);
}

std::string				 MyMsg::GetCmd( void )
{
	return (this->Command);
}

Clients					*MyMsg::GetClients( void )
{
	return (this->_SentFrom);
}

std::string 			MyMsg::GetPrefix( void )
{
	std::string tmp = "";

	if (this->_SentFrom->_Nickname.size())
	{
		tmp = ":" + this->_SentFrom->_Nickname;
		if (this->_SentFrom->_Nickname.size())
			tmp = tmp + "!" + this->_SentFrom->_Nickname;
		if (this->_SentFrom->_Nickname.size())
			tmp = tmp + "@" + this->_SentFrom->_Hostname;
		else
			tmp = tmp + "@" + this->_SentFrom->_HostAdress;
	}
	else
		tmp = ":" + this->_SentFrom->_ServerName;
	this->Prefix = tmp;
	return (this->Prefix);
}

int						MyMsg::GetCmdExistence( void )
{
	return (this->_DoesCmdExist);
}

	/*-------------------------------------------*/
	/*===========================================*/

std::string 			toupper_striing( std::string cmd )
{
	int i;
	std::string capitalized_str;

	i = -1;
	while (cmd[++i])
		capitalized_str[i] = toupper(cmd[i]);
	return (capitalized_str);
}

int						MyMsg::CheckFormatCmd( std::vector<std::string>::iterator cmd, std::vector<std::string> cmd_list )
{
	std::vector<std::string>::iterator 	it;
	int									i;
	unsigned long						j;
	
	it = cmd_list.begin();
	i = 0;
	j = 0;
	if (cmd->empty())
		return (FAILURE);
	if (isdigit(cmd->at(0)) != 0 && cmd->size() == 3)
	{
		while (i < 3)
		{
			if (isdigit(cmd->at(i)) == 0)
				return (FAILURE);
			i++;
		}
		return (SUCCESS);
	}
	while (j < cmd->size())
	{
		if (isalpha(cmd->at(j)) == 0)
			return (FAILURE);
		j++;
	}
	(void)cmd_list;
	return (SUCCESS);
}

int						MyMsg::ValidateClientsConnections( MyServer *IRC_Server )
{
	std::string intro_new_nick;

	this->_SentFrom->SetClientsConnectionPermission(YES);

	SendMsgBackWithPrefix(*this, ::RPL_WELCOME(*this));
	SendMsgBackWithPrefix(*this, ::RPL_YOURHOST(*this));
	SendMsgBackWithPrefix(*this, ::RPL_CREATED(*this));
	SendMsgBackWithPrefix(*this, ::RPL_MYINFO(*this));
	intro_new_nick = "\033[1;35mIntroducing new nick \033[1;37m" + this->_SentFrom->_Nickname + "\033[0m";
	SendMsgBackWithPrefix(*this, intro_new_nick);
	this->MotdCmd();
	(void)IRC_Server;
	return (SUCCESS);
}

bool 					MyMsg::Check_command(std::string str)
{
	if (str.size() == 0)
		return false;
	if (str.size() == 3 && isdigit(str[0]) == true)
	{
		for(int i = 0; i < 3; i++)
		{
			if (isdigit(str[i]) == false)
				return false;
		}
		return true;
	}
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isalpha(str[i]) == 0)
			return false;
	}
	return true;
}

bool					MyMsg::parse_msg(void)
{
	const char *delim = " ";
	char*	tmp = strdup(this->_Message.c_str());

	std::vector<std::string> 			tab_parse;
	std::vector<std::string>::iterator	str;

	char *token = strtok(tmp, delim);
	while(token != NULL)
	{
		tab_parse.push_back(std::string(token));
		token = strtok(NULL, delim);
	}
	free(tmp);

	if (tab_parse.size() < 1)
		return false;

	str = tab_parse.begin();
	if ((*str)[0] == ':')
	{
		this->Prefix = *str;
		++str;
	}
	if (Check_command(*str))
		this->Command = *str;
	else
	{
		std::cout << "Invalid command format." << std::endl;
		return false;
	}
	++str;
	if (str != tab_parse.end())
	{
		tab_parse.erase(tab_parse.begin(), str);
		this->SetParams2(tab_parse);
	}
	return true;
}