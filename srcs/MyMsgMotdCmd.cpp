# include "num_replies.cpp"

int			MyMsg::MotdCmd( void )
{
	std::string msg_sent;
	std::string file_tmp;
	std::vector<std::string> file_content;
	std::vector<std::string>::iterator it;

	if (this->Params.size() > 1) // S'il y'a trop de paramètres (verifier le nb de parametre sur les ordis de 42, moi j'en ai 4 bizarrement)
	{
		msg_sent = ERR_NOMOTD(*this, 1);
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	else
	{
		std::ifstream motd_file("./srcs/motd.txt");
		if (!motd_file.good()) // Si le fichier n'existe pas
		{
			msg_sent = ERR_NOMOTD(*this, 2);
			SendMsgBackWithPrefix(*this, msg_sent);
		}
		else // Si le fichier existe
		{
			msg_sent = RPL_MOTDSTART(*this); // Code pour commencer le msg
			SendMsgBackWithPrefix(*this, msg_sent);
			while (getline(motd_file, file_tmp)) // j'écris dans la variable le contenu de motd.txt
				file_content.push_back(file_tmp);
			it = file_content.begin();
			while (it != file_content.end())
			{
				msg_sent = RPL_MOTD(*this, it);
				SendMsgBackWithPrefix(*this, msg_sent); //je le renvoi ligne par ligne au client
				it++;
			}
		}
		motd_file.close();
		msg_sent = RPL_ENDOFMOTD(*this); // je ferme le stream et je renvoi le code signifiant la fin du msg
		SendMsgBackWithPrefix(*this, msg_sent);
	}
	return (SUCCESS);
}