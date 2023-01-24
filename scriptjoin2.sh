#!/bin/bash

# Connect to the IRC server

#echo "" | nc 127.0.0.1 6667

#for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
#do
 #   random_channel=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 8 | head -n 1)
	#channels+="$random_channels "
#	channels=$(printf "%s%s " "$channels" "$random_channels")

#done
#	echo $channels

	echo "PASS 111\nNICK adaloui\nUSER l l l l\nJOIN #lol\nOPER oper 1234\nTIME MyServerName\nNAMES\nNAMES #lol\nJOIN #a,#b,#c,#d,#e,#f,#g,#h,#i,#j,#k,#l,#m,#n,#o,#p,#q,#r,#s,#t,#u,#v,#w,#x,#y,#z,!a,!b,!c,!d,!e,!f,!g,!h,!i,!j,!k,!l,!m,!n,!o,!p,!q,!r,!s,!t,!u,!v,!w,!x,!y,!z,&a,&b,&c,&d,&e,&f,&g,&h,&i,&j,&k,&l,&m,&n,&o,&p,&q,&r,&s,&t,&u,&v,&w,&x,&y,&z,+a,+b,+c,+d,+e,+f,+g,+h,+i,+j,+k,+l,+m,+n,+o,+p,+q,+r,+s,+t,+u,+v,+w,+x,+y,+z\n" | nc 127.0.0.1 6667

