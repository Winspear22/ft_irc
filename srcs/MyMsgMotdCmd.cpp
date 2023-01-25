# include "num_replies.hpp"

int			MyMsg::MotdCmd( void )
{
	std::string msg_sent;
	std::string file_tmp;
	std::vector<std::string> file_content;
	std::vector<std::string>::iterator it;

	if (this->GetClients()->GetFirstMOTD() == 0) 
	{
		std::ifstream motd_file("./srcs/motd.txt");
		if (!motd_file.good()) 
		{
			msg_sent = ERR_NOMOTD(*this, 2);
			SendMsgBackWithPrefix(*this, msg_sent);
		}
		else 
		{
			msg_sent = RPL_MOTDSTART(*this); 
			SendMsgBackWithPrefix(*this, msg_sent);
			while (getline(motd_file, file_tmp))
			{ 
				if (file_tmp.length() > 80) 
					file_content.push_back(file_tmp.substr(0, 80).append("."));
				else							
					file_content.push_back(file_tmp);
			}
			it = file_content.begin();
			while (it != file_content.end())
			{
				msg_sent = RPL_MOTD(*this, it);
				SendMsgBackWithPrefix(*this, msg_sent); 
				it++;
			}
		}
		motd_file.close();
		msg_sent = RPL_ENDOFMOTD(*this);
		this->GetClients()->SetFirstMOTD(1);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	if (this->Params.size() == 1 && this->Params[0] == this->GetClients()->GetServerName() && this->GetClients()->GetFirstMOTD() == 1)
	{
		std::ifstream motd_file("./srcs/motd.txt");
		if (!motd_file.good()) 
		{
			msg_sent = ERR_NOMOTD(*this, 2);
			SendMsgBackWithPrefix(*this, msg_sent);
		}
		else 
		{
			msg_sent = RPL_MOTDSTART(*this); 
			SendMsgBackWithPrefix(*this, msg_sent);
			while (getline(motd_file, file_tmp))
			{ 
				if (file_tmp.length() > 80) 
					file_content.push_back(file_tmp.substr(0, 80).append("."));
				else							
					file_content.push_back(file_tmp);
			}
			it = file_content.begin();
			while (it != file_content.end())
			{
				msg_sent = RPL_MOTD(*this, it);
				SendMsgBackWithPrefix(*this, msg_sent); 
				it++;
			}
		}
		motd_file.close();
		msg_sent = RPL_ENDOFMOTD(*this); 
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else if (this->Params.empty() && this->GetClients()->GetFirstMOTD() == 1)
	{
		std::ifstream motd_file("./srcs/motd.txt");
		if (!motd_file.good()) 
		{
			msg_sent = ERR_NOMOTD(*this, 2);
			SendMsgBackWithPrefix(*this, msg_sent);
		}
		else 
		{
			msg_sent = RPL_MOTDSTART(*this); 
			SendMsgBackWithPrefix(*this, msg_sent);
			while (getline(motd_file, file_tmp))
			{ 
				if (file_tmp.length() > 80) 
					file_content.push_back(file_tmp.substr(0, 80).append("."));
				else							
					file_content.push_back(file_tmp);
			}
			it = file_content.begin();
			while (it != file_content.end())
			{
				msg_sent = RPL_MOTD(*this, it);
				SendMsgBackWithPrefix(*this, msg_sent); 
				it++;
			}
		}
		motd_file.close();
		msg_sent = RPL_ENDOFMOTD(*this); 
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	return (SUCCESS);
}