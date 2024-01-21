CC = cc -Wall -Wextra -pedantic -pthread -std=c99 -O2

.PHONY: make clean clear
make: concord concord/lib/libdiscord.a logdisc

logdisc.o: logdisc.c
	${CC}  -Iconcord/include -Iconcord/core -Iconcord/gencodecs \
               `pkg-config --cflags libcurl` \
		-c logdisc.c -o logdisc.o

logdisc: logdisc.o
	${CC}  -Lconcord/lib -L/usr/local/lib \
               `pkg-config --libs libcurl` \
	       -ldiscord logdisc.o -o logdisc

concord:
	git clone 'https://github.com/Cogmasters/concord.git'

concord/lib/libdiscord.a:
	CFLAGS=`pkg-config --cflags libcurl` \
	       LDFLAGS=`pkg-config --libs libcurl` \
	       gmake -C concord

clean clear: 
	rm -rf concord
	rm -rf logdisc logdisc.o
