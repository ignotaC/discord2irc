DBG=#-DNDEBUG
CC=cc -Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L -O0  ${DBG}
AR=ar rcs

IGDIR=ignotalib/src

.PHONY: make clean clear update

make: ircbot gethostipv lext fstrswp
	sh runircbot.sh

ircbot: libign_inetcli.a libigf_read.a libigf_write.a \
	libign_strtoport.a libigf_opt.a libigev_signals.a \
	libigt_sleep.a ircbot.c
	${CC} -c $@.c -o $@.o
	${CC} $@.o -L. -lign_inetcli -lign_strtoport \
	  -ligf_read -ligt_sleep -ligf_write -ligf_opt -ligev_signals \
	  -o $@

libign_inetcli.a: ignotalib ${IGDIR}/ig_net/ign_inetcli.c 
	${CC} -c ${IGDIR}/ig_net/ign_inetcli.c -o ign_inetcli.o
	${AR} $@ ign_inetcli.o

libigf_read.a: ignotalib ${IGDIR}/ig_file/igf_read.c 
	${CC} -c ${IGDIR}/ig_file/igf_read.c -o igf_read.o
	${AR} $@ igf_read.o

libigf_write.a: ignotalib ${IGDIR}/ig_file/igf_write.c 
	${CC} -c ${IGDIR}/ig_file/igf_write.c -o igf_write.o
	${AR} $@ igf_write.o

libign_strtoport.a: ignotalib ${IGDIR}/ig_net/ign_strtoport.c 
	${CC} -c ${IGDIR}/ig_net/ign_strtoport.c -o ign_strtoport.o
	${AR} $@ ign_strtoport.o

libigf_opt.a: ignotalib ${IGDIR}/ig_file/igf_opt.c 
	${CC} -c ${IGDIR}/ig_file/igf_opt.c -o igf_opt.o
	${AR} $@ igf_opt.o

libigev_signals.a: ignotalib ${IGDIR}/ig_event/igev_signals.c 
	${CC} -c ${IGDIR}/ig_event/igev_signals.c -o igev_signals.o
	${AR} $@ igev_signals.o

libigt_sleep.a: ignotalib ${IGDIR}/ig_time/igt_sleep.c 
	${CC} -c ${IGDIR}/ig_time/igt_sleep.c -o igt_sleep.o
	${AR} $@ igt_sleep.o



ignotalib:
	git clone 'https://github.com/ignotaC/ignotalib.git'

gethostipv: smallsoft
	cp smallsoft/bin/gethostipv .

lext: smallsoft
	cp smallsoft/bin/lext .


fstrswp: smallsoft
	cp smallsoft/bin/fstrswp .


smallsoft:
	git clone 'https://github.com/ignotaC/smallsoft.git'
	make -C 'smallsoft'

update:
	cd smallsoft; git pull
	cd ignotalib; git pull
	make make

clean clear:
	rm -rf ignotalib
	rm -rf smallsoft gethostipv lext fstrswp
	rm -rf igf_read.o libigf_read.a
	rm -rf igf_write.o libigf_write.a
	rm -rf igf_opt.o libigf_opt.a
	rm -rf igt_sleep.o libigt_sleep.a
	rm -rf ign_inetcli.o libign_inetcli.a
	rm -rf igev_signals.o libigev_signals.a
	rm -rf ign_strtoport.o libign_strtoport.a
	rm -rf ircbot ircbot.o




