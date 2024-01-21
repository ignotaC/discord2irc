DBG=#-DNDEBUG
CC=cc -Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L ${DBG}
AR=ar rcs

IGDIR=ignotalib/src

.PHONY: make clean clear

make: ircbot

ircbot: libign_inetcli.a libigf_read.a libigf_write.a ircbot.c
	${CC} -c $@.c -o $@.o
	${CC} $@.o -L. -lign_inetcli -ligf_read -ligf_write -o $@

libign_inetcli.a: ignotalib ${IGDIR}/ig_net/ign_inetcli.c 
	${CC} -c ${IGDIR}/ig_net/ign_inetcli.c -o ign_inetcli.o
	${AR} $@ ign_inetcli.o

libigf_read.a: ignotalib ${IGDIR}/ig_file/igf_read.c 
	${CC} -c ${IGDIR}/ig_file/igf_read.c -o igf_read.o
	${AR} $@ igf_read.o

libigf_write.a: ignotalib ${IGDIR}/ig_file/igf_write.c 
	${CC} -c ${IGDIR}/ig_file/igf_write.c -o igf_write.o
	${AR} $@ igf_write.o

ignotalib:
	git clone 'https://github.com/ignotaC/ignotalib.git'

clean clear:
	rm -rf ignotalib
	rm -rf igf_read.o libigf_read.a
	rm -rf igf_write.o libigf_write.a
	rm -rf ign_inetcli.o libign_inetcli.a
	rm -rf ircbot ircbot.o
