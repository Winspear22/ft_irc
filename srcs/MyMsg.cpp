# include "MyMsg.hpp"

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

std::string MyMsg::GetParams( void )
{
	return (this->_Params);
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
	this->_Params = Params;
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
