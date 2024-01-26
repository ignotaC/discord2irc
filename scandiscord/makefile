CC = cc -Wall -Wextra -pedantic -pthread -std=c99 -O2

.PHONY: make clean clear update


make: concord/lib/libdiscord.a logdisc
	@printf 'logdisc program builded\n'

logdisc.o: logdisc.c
	${CC}  -Iconcord/include -Iconcord/core -Iconcord/gencodecs \
               `pkg-config --cflags libcurl` \
		-c logdisc.c -o logdisc.o

logdisc: logdisc.o
	${CC}  logdisc.o -Lconcord/lib -L/usr/local/lib \
               -ldiscord `pkg-config --libs libcurl` \
	       -o logdisc

concord:
	git clone 'https://github.com/Cogmasters/concord.git'

concord/lib/libdiscord.a: concord
	CFLAGS=`pkg-config --cflags libcurl` \
	       LDFLAGS=`pkg-config --libs libcurl` \
	       gmake -C concord

update:
	cd concord; git pull
	make make

clean clear: 
	rm -rf concord
	rm -rf logdisc logdisc.o
