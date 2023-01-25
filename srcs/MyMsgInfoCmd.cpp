# include "num_replies.hpp"

std::string get_time_compilation()
{
    int         fd;
    struct stat file;
    std::string time;

    if ((fd = open("./ircserv", O_RDONLY)) > 0)
    {
        fstat(fd, &file);
        time = ctime(&file.st_mtime);
    }
	close(fd);
    return (time);
}

int		MyMsg::InfoCmd( void )
{
	std::string msg_sent;
	std::string file_tmp;
	std::vector<std::string> file_content;
	std::vector<std::string>::iterator it1;
	std::vector<std::string>::iterator it2;
	size_t			pos;
	std::string		str;
	std::string	time_compilation = get_time_compilation();

	it1 = this->Params.begin();
	if (this->Params.size() >= 1)
	{
		msg_sent = ERR_NOSUCHSERVER(*this, it1);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		std::ifstream info_file("./srcs/info.txt");
		if (!info_file.good()) 
			;
		else 
		{
			while (getline(info_file, file_tmp)) 
			{
				if (((pos = file_tmp.find("$date")) != std::string::npos))
				{
					str = file_tmp.substr(0, pos) + time_compilation + file_tmp.substr(pos + strlen("$date"));
					file_tmp = str;
				}
				file_content.push_back(file_tmp);
			}
			it2 = file_content.begin();
			while (it2 != file_content.end())
			{
				msg_sent = RPL_MOTD(*this, it2);
				SendMsgBackWithPrefix(*this, msg_sent);
				it2++;
			}
		}
		info_file.close();
		msg_sent = RPL_ENDOFINFO(*this);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	return (SUCCESS);
}