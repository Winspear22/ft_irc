#!/bin/bash

# Connect to the IRC server
#kill user42 :degage de mon serveur stp\r\n
#kick user42 :degage dici stp\r\n
#JOIN #lol\r\nOPER oper 1234\r\ntime MyServerName\r\nNAMES\r\nNAMES #lol\r\n
#PRIVMSG #lol :salut les amis comment ça va ?\r\n"
echo "PASS 111\r\nNICK adaloui\r\nUSER l l l l\r\nJOIN #lol\r\nOPER oper 1234\r\ntime MyServerName\r\nNAMES\r\nNAMES #lol\r\n
INVITE user42 #lol\r\nPRIVMSG #lol :salut les amis comment ça va ?\r\nkick user42 :degage dici stp\r\n
kill user42 :degage de mon serveur stp\r\n" | nc 127.0.0.1 6667 -C