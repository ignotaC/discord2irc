CC = cc -Wall -Wextra -pedantic -pthread -std=c99 -O2

.PHONY: make clean clear
make: concord concord/lib/libdiscord.a logdisc

logdisc.o: logdisc.c
	${CC}  -Iconcord/include -Iconcord/core -I/usr/local/include \
		-Iconcord/gencodecs \
		-c logdisc.c -o logdisc.o

logdisc: logdisc.o
	${CC}  -Lconcord/lib -L/usr/local/lib logdisc.o \
		-lcurl -ldiscord -o logdisc

concord:
	@echo get concord
	git clone 'https://github.com/Cogmasters/concord.git'

concord/lib/libdiscord.a:
	CFLAGS=-I/usr/local/include \
	       LDFLAGS=-L/usr/local/lib \
	       gmake -C concord

clean clear: 
	rm -rf concord
	rm -rf logdisc logdisc.o
